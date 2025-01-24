#include <gtest/gtest.h>
#include "crow.h"
#include "server.h"
#include <thread>
#include <future>
#include <chrono>
#include <curl/curl.h>
#include <unordered_map>
#include <sstream>
#include <iomanip>

// Función para realizar solicitudes HTTP con cURL (GET)
std::string http_request(const std::string &url)
{
    CURL *curl = curl_easy_init();
    if (!curl)
        return "";

    std::string response;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, +[](void *data, size_t size, size_t nmemb, void *userp)
                                                  {
        ((std::string*)userp)->append((char*)data, size * nmemb);
        return size * nmemb; });
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    return response;
}

// Función para convertir un unordered_map a un string con formato "key=value&key2=value2"
std::string map_to_post_data(const std::unordered_map<std::string, std::string> &params)
{
    std::ostringstream oss;
    for (auto it = params.begin(); it != params.end(); ++it)
    {
        if (it != params.begin())
        {
            oss << "&";
        }
        oss << curl_easy_escape(NULL, it->first.c_str(), it->first.length())
            << "="
            << curl_easy_escape(NULL, it->second.c_str(), it->second.length());
    }
    return oss.str();
}

// Función para realizar solicitudes HTTP POST con cURL
std::string http_post_request(const std::string &url, const std::unordered_map<std::string, std::string> &params)
{
    CURL *curl = curl_easy_init();
    if (!curl)
        return "";

    std::string response;

    // Convertir el unordered_map a datos POST
    std::string post_data = map_to_post_data(params);

    // Establecer URL y los datos POST
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());

    // Establecer los encabezados
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // Función para recibir la respuesta
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, +[](void *data, size_t size, size_t nmemb, void *userp)
                                                  {
        ((std::string*)userp)->append((char*)data, size * nmemb);
        return size * nmemb; });
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    // Realizar la solicitud
    curl_easy_perform(curl);

    // Limpiar
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    return response;
}

// Fixture para los tests del servidor Crow
class CrowServerTests : public ::testing::Test
{
protected:
    crow::SimpleApp app;
    std::thread server_thread;
    std::promise<void> server_ready;

    void SetUp() override
    {
        app.loglevel(crow::LogLevel::Warning);
        // Configurar rutas
        setup_routes(app);

        // Iniciar el servidor en un hilo separado
        server_thread = std::thread([&]()
                                    {
            server_ready.set_value();
            app.port(18080).run(); });

        // Esperar a que el servidor inicie
        server_ready.get_future().wait();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    void TearDown() override
    {
        // Detener el servidor
        app.stop();
        if (server_thread.joinable())
        {
            server_thread.join();
        }
    }
};

// Test específico usando la fixture
TEST_F(CrowServerTests, Hash256Route)
{
    // Hacer la solicitud HTTP y verificar la respuesta
    std::string response = http_request("http://localhost:18080/hash256/holamigos");
    // Parsear la respuesta JSON usando el soporte de Crow
    crow::json::rvalue json_response;
    try
    {
        json_response = crow::json::load(response); // Cargar el JSON desde la respuesta

        // Verificar que la respuesta contiene el campo "hash" con el valor esperado
        std::string expected_hash = "52213887221291bda14f3fe1ba591ebf8920c360d5233c9e64ff91812f65de0d";
        ASSERT_EQ(json_response["hash"].s(), expected_hash); // .s() devuelve un string
    }
    catch (const std::exception &e)
    {
        FAIL() << "Error parsing JSON: " << e.what();
    }
}

// Test específico para validar post request verify
TEST_F(CrowServerTests, Hash256RoutePostVerifyTrue)
{

    std::unordered_map<std::string, std::string> params = {
        {"hash", "52213887221291bda14f3fe1ba591ebf8920c360d5233c9e64ff91812f65de0d"},
        {"text", "holamigos"}};

    // Hacer la solicitud HTTP y verificar la respuesta
    std::string response = http_post_request("http://localhost:18080/hash256/verify", params);
    // Parsear la respuesta JSON usando el soporte de Crow
    crow::json::rvalue json_response;
    try
    {
        json_response = crow::json::load(response); // Cargar el JSON desde la respuesta

        // Verificar que la respuesta contiene el campo "hash" con el valor esperado
        ASSERT_TRUE(json_response["valid"].b()); // .s() devuelve un string
    }
    catch (const std::exception &e)
    {
        FAIL() << "Error parsing JSON: " << e.what();
    }
}

// Test específico para validar post request verify (false)
TEST_F(CrowServerTests, Hash256RoutePostVerifyFalse)
{

    std::unordered_map<std::string, std::string> params = {
        {"hash", "52213887221291bda14f3fe1ba591ebf8920c360d5233c9e64ff9181265de0d"},
        {"text", "holamigos"}};

    // Hacer la solicitud HTTP y verificar la respuesta
    std::string response = http_post_request("http://localhost:18080/hash256/verify", params);
    // Parsear la respuesta JSON usando el soporte de Crow
    crow::json::rvalue json_response;
    try
    {
        json_response = crow::json::load(response); // Cargar el JSON desde la respuesta

        // Verificar que la respuesta contiene el campo "hash" con el valor esperado
        ASSERT_FALSE(json_response["valid"].b()); // .s() devuelve un string
    }
    catch (const std::exception &e)
    {
        FAIL() << "Error parsing JSON: " << e.what();
    }
}