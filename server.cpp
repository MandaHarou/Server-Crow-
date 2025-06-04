#include "crow.h"
#include "./routes/login.route.hpp" 
#include "./routes/employer.route.hpp"

#include "./cors.middleware/cors.hpp"
#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
int main() {
    crow::App<CORS>app;
    mongocxx::instance instance{};
    mongocxx::client client{mongocxx::uri{}};
    auto db = client["gestion_personnel"];

    loginRoute(app, db);         
    EmployeRoutes(app, db);  

    std::cout << "Serveur lancé sur http://localhost:18080" << std::endl;
    app.port(18080).multithreaded().run();
}
