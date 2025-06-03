#pragma once
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/database.hpp>
#include <iostream>

namespace adminmodel {
    bool insertUser(mongocxx::database& db, const std::string& nom, const std::string& password) {
        try {
            auto collection = db["admin"];
            bsoncxx::builder::stream::document document{};
            document << "nom" << nom << "password" << password;
            collection.insert_one(document.view());
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Erreur MongoDB: " << e.what() << std::endl;
            return false;
        }
    }
}
