#pragma once
#include "crow.h"
#include "../model/admin.model.hpp"

namespace logcontroler {
    crow::response login(const crow::request& req, crow::response& res, mongocxx::database& db) {
        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400, "JSON invalide");

        std::string name = body["name"].s();
        std::string password = body["password"].s();

        
        const std::string adminName = "admin";
        const std::string adminPass = "admin123";

        if (name == adminName && password == adminPass) {
            return crow::response(200, "Connexion admin réussie");
        } else {
            // Création d'utilisateur seulement si échec d'authentification
            if (adminmodel::insertUser(db, name, password)) {
                return crow::response(200, "Nouvel utilisateur enregistré !");
            } else {
                return crow::response(500, "Erreur lors de l'ajout");
            }
        }
    }
}
