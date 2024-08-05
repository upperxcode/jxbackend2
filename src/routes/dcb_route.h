#pragma once
#include "../models/dcb_model.h"
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
namespace dcb {



inline auto fetchAll = [](const crow::request &req) {
    PostgresDatabase &conn = DatabaseSingleton::getInstance();
    // Obtém a lista de dcbs
    try {
        std::vector<Dcb> dcbs = query::select<Dcb>(conn);

        crow::json::wvalue response;
        crow::json::wvalue::list cJson;

        for (auto &j : dcbs) {
            cJson.push_back(record::toJson(j));
        }

        response["dcbs"] = std::move(cJson);
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
    Dcb dcb = query::select<Dcb>(conn, id)[0];

    auto json = record::toJson(dcb);
    response["dcbs"] = std::move(json);

    
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

  Dcb dcb;
  try {
    record::fromJson(dcb, json);
    PostgresDatabase &conn = DatabaseSingleton::getInstance();
    query::insert(conn, dcb);
    return crow::response(record::toJson(dcb));
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

  Dcb dcb;
  try {
    record::fromJson(dcb, json);
    dcb.id = id;
    PostgresDatabase &conn = DatabaseSingleton::getInstance();
    query::update(conn, dcb);
    return crow::response(record::toJson(dcb));
  } catch (const std::exception &e) {
    crow::json::wvalue errorResponse;
    errorResponse["error"] = e.what();
    return crow::response(500, errorResponse);
  }
};

inline auto remove = [](const crow::request &req, int id) {
  Dcb dcb;
  dcb.id = id;
  try {

    PostgresDatabase &conn = DatabaseSingleton::getInstance();
    query::remove(conn, dcb, id);
    return crow::response(record::toJson(dcb));
  } catch (const std::exception &e) {
    crow::json::wvalue errorResponse;
    errorResponse["error"] = e.what();
    return crow::response(500, errorResponse);
  }
};

  inline void init(crow::SimpleApp &app) {

    http::Get(app, "/dcbs", fetchAll);
    http::Get(app, "/dcb/<int>", fetchOne);
    http::Post(app, "/dcb", insert);
    http::Put(app, "/dcb/<int>", update);
    http::Delete(app, "/dcb/<int>", remove);
  }

} // namespace dcb
} // namespace route