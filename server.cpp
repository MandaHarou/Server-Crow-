#include "crow.h"
#include "./routes/login.route.hpp"
#include "./routes/employer.route.hpp"
#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>
int main() {
    crow::SimpleApp app;
    mongocxx::instance instance{};
    mongocxx::client client{mongocxx::uri{}};
    auto db = client["gestion_personnel"];

    addroutes(app, db);         
    addEmployeRoutes(app, db);  

    std::cout << "Serveur lancé sur http://localhost:18080" << std::endl;
    app.port(18080).multithreaded().run();
}
