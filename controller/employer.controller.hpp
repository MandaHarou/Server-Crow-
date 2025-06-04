#pragma once
#include "crow.h"
#include "../model/user.model.hpp"

namespace employecontroler {
    crow::response ajouter(const crow::request& req, mongocxx::database& db) {
        auto body = crow::json::load(req.body);
        if (!body)
            return crow::response(400, "Données JSON invalides");

        std::string nom = body["nom"].s();
        std::string email = body["email"].s();
        std::string poste = body["poste"].s();
        std::string affectation = body["affectation"].s();
        int conger = body["conger"].i();

        
        if (employemodel::insertEmploye(db, nom, email,poste,affectation,conger)) {
            std::cout << "Insertion réussie dans la base MongoDB.\n";
            return crow::response(201, "Employé ajouté");
        } else {
              std::cerr << "Échec de l'insertion dans la base MongoDB.\n";
            return crow::response(500, "Erreur lors de l'ajout");
        }
        return crow::response(500, "Erreur serveur");

    }

   crow::response lister(mongocxx::database& db) {
    crow::json::wvalue result;
    int i = 0;

    for (auto doc : employemodel::getAllEmployes(db)) {
        crow::json::wvalue emp;
        emp["id"] = doc["_id"].get_oid().value.to_string();
        emp["nom"] = std::string(doc["nom"].get_string().value);
        emp["email"] = std::string(doc["email"].get_string().value);
        emp["poste"] = std::string(doc["poste"].get_string().value);
        emp["affectation"] = std::string(doc["affectation"].get_string().value);

        // Handle different types for conger
        try {
            auto conger = doc["conger"];
            if (conger.type() == bsoncxx::type::k_int32) {
                emp["conger"] = conger.get_int32().value;
            } else if (conger.type() == bsoncxx::type::k_int64) {
                emp["conger"] = static_cast<int>(conger.get_int64().value); // Convert int64 to int
            } else if (conger.type() == bsoncxx::type::k_double) {
                emp["conger"] = static_cast<int>(conger.get_double().value); // Convert double to int
            } else if (conger.type() == bsoncxx::type::k_string) {
                emp["conger"] = std::stoi(std::string(conger.get_string().value)); // Convert string to int
            } else {
                std::cerr << "Unexpected type for conger: " << static_cast<int>(conger.type()) << std::endl;
                return crow::response(500, "Type de données inattendu pour conger");
            }
        } catch (const std::exception& e) {
            std::cerr << "Error processing conger: " << e.what() << std::endl;
            return crow::response(500, "Erreur lors du traitement du champ conger");
        }

        result[i++] = std::move(emp);
    }

    return crow::response(200, result);
}
crow::response modifier(const crow::request& req, mongocxx::database& db, const std::string& id) {
    auto body = crow::json::load(req.body);
    if (!body)
        return crow::response(400, "Données JSON invalides");

    bsoncxx::oid obj_id;
    try {
        obj_id = bsoncxx::oid(id);
    } catch (...) {
        return crow::response(400, "ID invalide");
    }

    bsoncxx::builder::stream::document filter_builder, update_builder;
    filter_builder << "_id" << obj_id;
    update_builder << "$set" << bsoncxx::builder::stream::open_document
                   << "nom" << body["nom"].s()
                   << "email" << body["email"].s()
                   << "poste" << body["poste"].s()
                   << "affectation" << body["affectation"].s()
                   << "conger" << body["conger"].i()
                   << bsoncxx::builder::stream::close_document;

    auto result = db["employes"].update_one(filter_builder.view(), update_builder.view());

    if (result && result->modified_count() == 1)
        return crow::response(200, "Employé modifié avec succès");
    else
        return crow::response(404, "Aucun employé trouvé ou pas de modification");
}
crow::response supprimer(mongocxx::database& db, const std::string& id) {
    bsoncxx::oid obj_id;
    try {
        obj_id = bsoncxx::oid(id);
    } catch (...) {
        return crow::response(400, "ID invalide");
    }

    bsoncxx::builder::stream::document filter_builder;
    filter_builder << "_id" << obj_id;

    auto result = db["employes"].delete_one(filter_builder.view());

    if (result && result->deleted_count() == 1)
        return crow::response(200, "Employé supprimé avec succès");
    else
        return crow::response(404, "Employé non trouvé");
}
crow::response afficher(mongocxx::database& db, const std::string& id) {
    bsoncxx::oid obj_id;
    try {
        obj_id = bsoncxx::oid(id);
    } catch (...) {
        return crow::response(400, "ID invalide");
    }

    auto result = db["employes"].find_one(bsoncxx::builder::stream::document{} << "_id" << obj_id << bsoncxx::builder::stream::finalize);

    if (!result)
        return crow::response(404, "Employé non trouvé");

    auto doc = result->view();
    crow::json::wvalue emp;
    emp["id"] = doc["_id"].get_oid().value.to_string();
    emp["nom"] = std::string(doc["nom"].get_string().value);
    emp["email"] = std::string(doc["email"].get_string().value);
    emp["poste"] = std::string(doc["poste"].get_string().value);
    emp["affectation"] = std::string(doc["affectation"].get_string().value);

    // Handle different types for conger
    try {
        auto conger = doc["conger"];
        if (conger.type() == bsoncxx::type::k_int32) {
            emp["conger"] = conger.get_int32().value;
        } else if (conger.type() == bsoncxx::type::k_int64) {
            emp["conger"] = static_cast<int>(conger.get_int64().value); // Convert int64 to int
        } else if (conger.type() == bsoncxx::type::k_double) {
            emp["conger"] = static_cast<int>(conger.get_double().value); // Convert double to int
        } else if (conger.type() == bsoncxx::type::k_string) {
            emp["conger"] = std::stoi(std::string(conger.get_string().value)); // Convert string to int
        } else {
            std::cerr << "Unexpected type for conger: " << static_cast<int>(conger.type()) << std::endl;
            return crow::response(500, "Type de données inattendu pour conger");
        }
    } catch (const std::exception& e) {
        std::cerr << "Error processing conger: " << e.what() << std::endl;
        return crow::response(500, "Erreur lors du traitement du champ conger");
    }

    return crow::response(200, emp);
}
}