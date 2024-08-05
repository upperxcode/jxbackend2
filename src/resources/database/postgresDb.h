
#pragma once

#include "database.h"
#include <iostream>
#include <pqxx/pqxx>
#include <exceptions/exceptions.h>

class PostgresDatabase : public Database {
public:
  PostgresDatabase(const std::string &connectionString)
      : conn(connectionString) {}

  Result queryRow(const std::string &query) override {
    pqxx::work txn(conn);
    pqxx::result res = txn.exec(query);

     // Verifica se a consulta retornou resultados
        if (res.empty()) {
            throw EmptyResultException(query);
        }
  

    Result result;
    for (const auto &row : res) {
      Row resultRow;
      for (const auto &field : row)
        resultRow[field.name()] = field.c_str();
      result.push_back(resultRow);
    }
   
    return result;
  }

   Result queryRows(const std::string &query) override {
    pqxx::work txn(conn);
    pqxx::result res = txn.exec(query);

    Result result;
    for (const auto &row : res) {
      Row resultRow;
      for (const auto &field : row)
        resultRow[field.name()] = field.c_str();
      result.push_back(resultRow);
    }
    txn.commit();
    return result;
  }

  int exec(const std::string &query) override {
    pqxx::work txn(conn);

    try {
      pqxx::result res = txn.exec(query);

      txn.commit();
      return res.affected_rows();
    }

    catch (const std::exception &e) {
      std::cerr << e.what() << std::endl;
    }

    return 0;
  }

  const char *dbName() {
    if (conn.is_open())
      return conn.dbname();
    else
      return "";
  }

private:
  pqxx::connection conn;
};
