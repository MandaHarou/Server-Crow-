#pragma once
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <string>

struct User {
    std::string nom;
    std::string poste;
    std::string conge;
    std::string email;

    bsoncxx::document::value to_bson() const {
        using namespace bsoncxx::builder::stream;
        return document{} << "nom" << nom << "poste" << poste << "conge" << conge << "email"<< email<< finalize;
    }
};