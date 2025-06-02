#include<iostream>
#include"crow.h"

int main(){
    crow::SimpleApp app;
    CROW_ROUTE(app, "/")([](){
        return "Api route crow";
    });
    CROW_ROUTE(app, "/json").methods("POST"_method)([](const crow::request& req){
        auto body = crow::json::load(req.body);
        if(!body){
            return crow::response(400,"Json valide !");
        }
        std::string nom = body ["nom"].s();
        int age  = body["age"].i();
        crow::json::wvalue result;
      
        result["age"] = age;
        result["status"] = "reçu avec succès";

        return crow::response{result};

    });
    app.port(5000).multithreaded().run();

    return 0;
}