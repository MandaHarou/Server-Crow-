#pragma once
#include "crow.h"
#include "../controller/employer.controller.hpp"
#include "../cors.middleware/cors.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;
void EmployeRoutes(crow::App<CORS>& app, mongocxx::database& db) {
    using namespace employecontroler;

    CROW_ROUTE(app, "/employes/add").methods("POST"_method)([&db](const crow::request& req) {
        return ajouter(req, db);
    });

    CROW_ROUTE(app, "/employes/all").methods("GET"_method)([&db]() {
        return lister(db);
    });

    CROW_ROUTE(app, "/employes/<string>").methods("GET"_method)([&db](const std::string& id) {
        return afficher(db, id);
    });

    CROW_ROUTE(app, "/employes/update/<string>").methods("PUT"_method)([&db](const crow::request& req, const std::string& id) {
        return modifier(req, db, id);
    });

    CROW_ROUTE(app, "/employes/delete/<string>").methods("DELETE"_method)([&db](const std::string& id) {
        return supprimer(db, id);
    });
}
