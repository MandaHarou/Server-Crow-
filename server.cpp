#include "crow.h"
#include "config/db.hpp"
#include "routes/login.route.hpp"

int main() {
    crow::SimpleApp app;
    auto database = db::connect("gestionpersonnel");
    addroutes(app, database);
    app.port(18080).multithreaded().run();
}
