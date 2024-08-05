#ifndef DB_H
#define DB_H

#include "postgresDb.h"
#include "record.h"
#include <crow/json.h> // Biblioteca JSON
#include <ctime>
#include <database/field>
#include <format>
#include <iostream>
#include <string>
#include <utils/print.h>
#include <vector>

using namespace std;
using sql = record::SQLQueryType;

namespace query {

// Função para inserir um objeto no banco de dados usando PostgresDatabase
template <typename T> bool insert(PostgresDatabase &db, const T &record) {
  std::string sql = record::generateSQLQuery(record, sql::INSERT);
  std::cout << sql << std::endl;

  if (db.exec(sql) > 0)
    return true;

  return false;
}

// Função para selecionar registros do banco de dados usando PostgresDatabase
template <typename T>
std::vector<T> select(PostgresDatabase &db,
                      const std::string &conditions = "") {
  std::string sql = record::generateSQLQuery(T{}, sql::SELECT, conditions);
  auto result = db.queryRows(sql);
  return record::getRecords<T>(result);
}

template <typename T>
std::vector<T> select(PostgresDatabase &db, const int id) {
  std::string sql =
      record::generateSQLQuery(T{}, sql::SELECT, std::format(" id = {} ", id));
  auto result = db.queryRow(sql);
  return record::getRecords<T>(result);
}

template <typename T> bool update(PostgresDatabase &db, const T &record) {

  std::string sql = record::generateSQLQuery(record, sql::UPDATE);

  LOG_DEBUG(sql);
  if (db.exec(sql) > 0)
    return true;

  return false;
}

// Função para deletar registros do banco de dados usando PostgresDatabase
template <typename T>
bool remove(PostgresDatabase &db, const T &record, int id) {

  std::string sql = record::generateSQLQuery(record, sql::DELETE);
  LOG_DEBUG(sql);
  if (db.exec(sql) > 0)
    return true;

  return false;
}

} // namespace query

#endif // DB_H
