// src/server.cpp
#include "server.h"
#include "hash256.h"
#include <sstream>
#include "OpenSSLAdapter.h"

std::string calculate_hash256(const std::string &text)
{
    // Crear una instancia de OpenSSLAdapter
    auto opensslAdapter = std::make_shared<OpenSSLAdapter>();

    // Pasar el adaptador a Hash256
    Hash256 hasher(opensslAdapter);
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

    CROW_ROUTE(app, "/hash256/verify")
        .methods(crow::HTTPMethod::POST)(
            [](const crow::request &req)
            {
                crow::json::wvalue result;

                try
                {
                    // Obtener parámetros del cuerpo de la solicitud
                    auto params = req.get_body_params();

                    // // Validar que los parámetros necesarios están presentes
                    // if (!params.get("hash") || !params.get("text"))
                    // {
                    //     result["error"] = "Missing required parameters: 'hash' and/or 'text'";
                    //     return crow::response(400, result); // 400 Bad Request
                    // }

                    std::string hash{params.get("hash")};
                    std::string text{params.get("text")};

                    // Verificar el hash con la clase Hash256
                    // Crear una instancia de OpenSSLAdapter
                    auto opensslAdapter = std::make_shared<OpenSSLAdapter>();

                    // Pasar el adaptador a Hash256
                    Hash256 hash256(opensslAdapter);

                    result["valid"] = hash256.verifyHexHash(text, hash);
                    result["hash"] = hash;
                    result["text"] = text;

                    return crow::response(200, result); // 200 OK
                }
                catch (const std::exception &e)
                {
                    // Manejo de excepciones generales
                    result["error"] = "Internal Server Error: " + std::string(e.what());
                    return crow::response(500, result); // 500 Internal Server Error
                }
            });
}
