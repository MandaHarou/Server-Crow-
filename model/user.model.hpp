#pragma once
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <string>

namespace employemodel {
   bool insertEmploye(mongocxx::database& db, const std::string& nom, const std::string& email,
                   const std::string& poste, const std::string& affectation, int conger) {
    try {
        bsoncxx::builder::stream::document document{};
        document << "nom" << nom
                 << "email" << email
                 << "poste" << poste
                 << "affectation" << affectation
                 << "conger" << conger;

        auto collection = db["employes"];
        collection.insert_one(document.view());
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Erreur MongoDB : " << e.what() << std::endl;
        return false;
    }
}

   mongocxx::cursor getAllEmployes(mongocxx::database& db) {
        return db["employes"].find({});
    }
}