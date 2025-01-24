#include <gtest/gtest.h>
#include "crow.h"
#include "server.h"
#include <thread>
#include <future>
#include <chrono>
#include <curl/curl.h>

// Función para realizar solicitudes HTTP con cURL
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

// Fixture para los tests del servidor Crow
class CrowServerTests : public ::testing::Test
{
protected:
    crow::SimpleApp app;
    std::thread server_thread;
    std::promise<void> server_ready;

    void SetUp() override
    {
        // Configurar rutas
        setup_routes(app);

        // Iniciar el servidor en un hilo separado
        server_thread = std::thread([&]()
                                    {
            server_ready.set_value();
            app.port(18080).run(); });

        // Esperar a que el servidor inicie
        server_ready.get_future().wait();
        std::this_thread::sleep_for(std::chrono::seconds(1));
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
    EXPECT_EQ(response, "{\"hash\":\"52213887221291bda14f3fe1ba591ebf8920c360d5233c9e64ff91812f65de0d\"}");
}