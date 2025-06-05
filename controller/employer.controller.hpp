#pragma once
#include "crow.h"
#include "../model/user.model.hpp"
#include "../cors.middleware/cors.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;
namespace employecontroler {
    crow::response ajouter(const crow::request& req, mongocxx::database& db) {
     
      json body;
    try {
        body = json::parse(req.body);
    } catch (const json::parse_error& e) {
        return crow::response(400, "JSON invalide: " + std::string(e.what()));
    }
           std::string nom, email, poste,affectation,conger;
         
try{
        nom = body["nom"].get<std::string>();
        email = body["email"].get<std::string>();
        poste = body["poste"].get<std::string>();
        affectation = body["affectation"].get<std::string>();
        conger = body["conger"].get<std::string>();
       
} catch (const json::exception& e) {
        return crow::response(400, "Erreur dans les types des champs: " + std::string(e.what()));
    }
        
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
    json result = json::array();
  
    for (auto doc : employemodel::getAllEmployes(db)) {
        json emp;
        emp["id"] = doc["_id"].get_oid().value.to_string();
        emp["nom"] = std::string(doc["nom"].get_string().value);
        emp["email"] = std::string(doc["email"].get_string().value);
        emp["poste"] = std::string(doc["poste"].get_string().value);
        emp["affectation"] = std::string(doc["affectation"].get_string().value);
        emp["conger"] = std::string(doc["conger"].get_string().value);
        result.push_back(emp);
    }

    return crow::response(200, result.dump());
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
                   << "conger" << body["conger"].s()
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
    emp["conger"] = std::string(doc["conger"].get_string().value);
    return crow::response(200, emp);
}
}