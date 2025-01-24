// src/server.cpp
#include "server.h"
#include "hash256.h"
#include <sstream>

std::string calculate_hash256(const std::string &text)
{
    Hash256 hasher;
    return hasher.calculateHex(text);
}

std::string url_decode(const std::string &src)
{
    std::string dest;
    dest.reserve(src.length());
    for (size_t i = 0; i < src.length(); ++i)
    {
        if (src[i] == '%' && i + 2 < src.length())
        {
            int value;
            std::istringstream ss(src.substr(i + 1, 2)); // Corregido: declaración del 'istringstream'
            ss >> std::hex >> value;                     // Usar 'ss' para leer el valor hexadecimal
            dest.push_back(static_cast<char>(value));
            i += 2; // Avanzar 2 posiciones, porque ya hemos procesado los dos caracteres hexadecimales
        }
        else
        {
            dest.push_back(src[i]);
        }
    }
    return dest;
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
                // Decodificar el texto recibido
                text = url_decode(text);

                std::string hash = calculate_hash256(text);
                crow::json::wvalue result;
                result["input"] = text;
                result["hash"] = hash;
                return crow::response(200, result);
            }
            catch (const std::exception &e)
            {
                crow::json::wvalue result;
                result["Error"] = e.what();
                return crow::response(500, result);
            }
        });
}
