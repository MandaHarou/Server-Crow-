#pragma once
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <iostream>

namespace db {
    inline mongocxx::instance instance{};
    inline mongocxx::client client{mongocxx::uri{"mongodb://localhost:27017"}};

    inline mongocxx::database connect(const std::string& bdname) {
        try {
            auto db = client[bdname];
            std::cout << "[+] Connexion à MongoDB réussie." << std::endl;
            return db;
        } catch (const std::exception& e) {
            std::cerr << "[-] Erreur de connexion à MongoDB : " << e.what() << std::endl;
            throw;
        }
    }
}
