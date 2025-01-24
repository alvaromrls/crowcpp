#include <gtest/gtest.h>
#include "crow.h"
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

TEST(CrowServerTests, RootRoute)
{
    crow::SimpleApp app;

    // Configurar rutas
    app.route_dynamic("/")([]()
                           { return "Hello, Test!"; });

    // Ejecutar el servidor en un hilo separado
    std::promise<void> server_ready;
    std::thread server_thread([&]()
                              {
        server_ready.set_value();
        app.port(18080).run(); });

    // Esperar a que el servidor inicie
    server_ready.get_future().wait();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Hacer la solicitud HTTP
    std::string response = http_request("http://localhost:18080/");
    EXPECT_EQ(response, "Hello, Test!");

    // Detener el servidor
    app.stop();
    server_thread.join();
}
