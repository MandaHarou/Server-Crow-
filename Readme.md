# C++ Project 
## API RESTApi CROW/BoostBest
End point
- `AddEmployer`:
```c++
    CROW_ROUTE(app, "/employes/add").methods("POST"_method)([&db](const crow::request& req) {
        return ajouter(req, db);
    });
```

