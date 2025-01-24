// src/server.cpp
#include "server.h"

void setup_routes(crow::SimpleApp &app)
{
    app.route_dynamic("/")([]()
                           { return "Hello, World!"; });
}
