#pragma once
#include "crow.h"
#include "../controller/log.controler.hpp"
#include "../cors.middleware/cors.hpp"

void loginRoute(crow::App<CORS>& app, mongocxx::database& db) {
    using namespace logcontroler;
    CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::Post)
        ([&db](const crow::request& req) {
            return login(req,db); 
        });
}