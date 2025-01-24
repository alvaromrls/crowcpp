// src/server.cpp
#include "server.h"
#include "hash256.h"

std::string calculate_hash256(const std::string &text)
{
    Hash256 hasher;
    return hasher.calculateHex(text);
}

void setup_routes(crow::SimpleApp &app)
{
    app.route_dynamic("/")([]()
                           { return "Hello, World!"; });

    // Ruta para calcular hash256 de un texto
    CROW_ROUTE(app, "/hash256/<string>")
    (
        [](std::string text)
        {
            try
            {
                std::string hash = calculate_hash256(text);
                return crow::response(200, hash);
            }
            catch (const std::exception &e)
            {
                return crow::response(500, std::string("Error: ") + e.what());
            }
        });
}
