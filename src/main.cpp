// src/main.cpp
#include "crow.h"
#include "server.h"

int main()
{
    crow::SimpleApp app;
    setup_routes(app);
    app.port(8080).run();
    return 0;
}
