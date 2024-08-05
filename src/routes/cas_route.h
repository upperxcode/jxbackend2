#pragma once
#include "../models/cas_model.h"
#include "crow/http_response.h"
#include "crow/json.h"
#include "database/databaseSingleto.h"
#include "database/query.h"
#include "database/record.h"
#include <crow/app.h>
#include <http.h>

#include <cstdio>
#include <exceptions/database_exception.h>

namespace route {
namespace cas {



inline auto fetchAll = [](const crow::request &req) {
    PostgresDatabase &conn = DatabaseSingleton::getInstance();
    // Obtém a lista de cass
    try {
        std::vector<Cas> cass = query::select<Cas>(conn);

        crow::json::wvalue response;
        crow::json::wvalue::list cJson;

        for (auto &j : cass) {
            cJson.push_back(record::toJson(j));
        }

        response["cass"] = std::move(cJson);
        return response;

    } catch (const std::exception &e) {
        crow::json::wvalue errorResponse;
        errorResponse["error"] = e.what();
        return errorResponse;
    }
};

inline auto fetchOne = [](const crow::request &req, int id) {
  PostgresDatabase &conn = DatabaseSingleton::getInstance();

  crow::json::wvalue response;
 
  try {
    Cas cas = query::select<Cas>(conn, id)[0];

    auto json = record::toJson(cas);
    response["cass"] = std::move(json);

    
  } catch (const DatabaseException &e) {
    response["error"] = e.msg();
  }

  // auto cJson = crow::json::wvalue({jsonString});

  return response;
};

inline auto insert = [](const crow::request &req) {
  auto json = crow::json::load(req.body);
  if (!json) {
    return crow::response(400, "Invalid JSON");
  }

  Cas cas;
  try {
    record::fromJson(cas, json);
    PostgresDatabase &conn = DatabaseSingleton::getInstance();
    query::insert(conn, cas);
    return crow::response(record::toJson(cas));
  } catch (const std::exception &e) {
    crow::json::wvalue errorResponse;
    errorResponse["error"] = e.what();
    return crow::response(500, errorResponse);
  }
};

inline auto update = [](const crow::request &req, int id) {
  auto json = crow::json::load(req.body);
  if (!json) {
    return crow::response(400, "Invalid JSON");
  }

  Cas cas;
  try {
    record::fromJson(cas, json);
    cas.id = id;
    PostgresDatabase &conn = DatabaseSingleton::getInstance();
    query::update(conn, cas);
    return crow::response(record::toJson(cas));
  } catch (const std::exception &e) {
    crow::json::wvalue errorResponse;
    errorResponse["error"] = e.what();
    return crow::response(500, errorResponse);
  }
};

inline auto remove = [](const crow::request &req, int id) {
  Cas cas;
  cas.id = id;
  try {

    PostgresDatabase &conn = DatabaseSingleton::getInstance();
    query::remove(conn, cas, id);
    return crow::response(record::toJson(cas));
  } catch (const std::exception &e) {
    crow::json::wvalue errorResponse;
    errorResponse["error"] = e.what();
    return crow::response(500, errorResponse);
  }
};

  inline void init(crow::SimpleApp &app) {

    http::Get(app, "/cass", fetchAll);
    http::Get(app, "/cas/<int>", fetchOne);
    http::Post(app, "/cas", insert);
    http::Put(app, "/cas/<int>", update);
    http::Delete(app, "/cas/<int>", remove);
  }

} // namespace cas
} // namespace route