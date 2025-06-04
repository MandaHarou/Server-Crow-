#pragma once
#include "crow.h"
#include "../model/admin.model.hpp"
#include "../cors.middleware/cors.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace logcontroler {
    crow::response login(const crow::request& req, mongocxx::database& db) {
        json body;
        try {
            body = json::parse(req.body);
        } catch (const json::parse_error& e) {
            return crow::response(400, json{{"message", "JSON invalide: " + std::string(e.what())}}.dump());
        }

        if (!body.contains("name") || !body.contains("password")) {
            return crow::response(400, json{{"message", "Champs 'name' ou 'password' manquants"}}.dump());
        }

        std::string name;
        std::string password;
       
            name = body["name"].get<std::string>();
            password = body["password"].get<std::string>();
      

        const std::string adminName = "admin";
        const std::string adminPass = "admin123";

        if (name == adminName && password == adminPass) {
            std::cout<<"succes";
            json response = {{"message", "Connexion admin réussie"}};
            return crow::response(200, response.dump());
        } else {
            if (adminmodel::insertUser(db, name, password)) {
                json response = {{"message", "Nouvel utilisateur enregistré !"}};
                return crow::response(200, response.dump());
            } else {
                json response = {{"message", "Erreur lors de l'ajout"}};
                return crow::response(500, response.dump());
            }
        }
    }
}