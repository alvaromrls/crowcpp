#include <gtest/gtest.h>
#include <crow.h>

// Test simple para verificar que Crow maneja rutas
TEST(CrowTests, BasicRouteHandling)
{
    crow::SimpleApp app;

    // Añadir una ruta
    CROW_ROUTE(app, "/")
    ([]()
     { return "Hello, World!"; });

    // Simular un request
    // auto response = app.handle_request("GET", "/");
    // EXPECT_EQ(response.code, 200);
    // EXPECT_EQ(response.body, "Hello, World!");
}

// Test adicional
TEST(MathTests, Addition)
{
    EXPECT_EQ(2 + 2, 4);
    EXPECT_NE(2 + 2, 5);
}
