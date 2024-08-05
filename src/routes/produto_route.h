#pragma once
#include "../models/produto_model.h"
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
namespace produto {



inline auto fetchAll = [](const crow::request &req) {
    PostgresDatabase &conn = DatabaseSingleton::getInstance();
    // Obtém a lista de produtos
    try {
        std::vector<Produto> produtos = query::select<Produto>(conn);

        crow::json::wvalue response;
        crow::json::wvalue::list cJson;

        for (auto &j : produtos) {
            cJson.push_back(record::toJson(j));
        }

        response["produtos"] = std::move(cJson);
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
    Produto produto = query::select<Produto>(conn, id)[0];

    auto json = record::toJson(produto);
    response["produtos"] = std::move(json);

    
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

  Produto produto;
  try {
    record::fromJson(produto, json);
    PostgresDatabase &conn = DatabaseSingleton::getInstance();
    query::insert(conn, produto);
    return crow::response(record::toJson(produto));
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

  Produto produto;
  try {
    record::fromJson(produto, json);
    produto.id = id;
    PostgresDatabase &conn = DatabaseSingleton::getInstance();
    query::update(conn, produto);
    return crow::response(record::toJson(produto));
  } catch (const std::exception &e) {
    crow::json::wvalue errorResponse;
    errorResponse["error"] = e.what();
    return crow::response(500, errorResponse);
  }
};

inline auto remove = [](const crow::request &req, int id) {
  Produto produto;
  produto.id = id;
  try {

    PostgresDatabase &conn = DatabaseSingleton::getInstance();
    query::remove(conn, produto, id);
    return crow::response(record::toJson(produto));
  } catch (const std::exception &e) {
    crow::json::wvalue errorResponse;
    errorResponse["error"] = e.what();
    return crow::response(500, errorResponse);
  }
};

  inline void init(crow::SimpleApp &app) {

    http::Get(app, "/produtos", fetchAll);
    http::Get(app, "/produto/<int>", fetchOne);
    http::Post(app, "/produto", insert);
    http::Put(app, "/produto/<int>", update);
    http::Delete(app, "/produto/<int>", remove);
  }

} // namespace produto
} // namespace route