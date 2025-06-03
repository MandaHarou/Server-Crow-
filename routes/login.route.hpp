#pragma once
#include "crow.h"
#include "../controller/log.controler.hpp"

void addroutes(crow::SimpleApp& app, mongocxx::database& db) {
    CROW_ROUTE(app, "/login").methods("POST"_method)([&db](const crow::request& req) {
        crow::response res;
        return logcontroler::login(req, res, db);
    });
}
