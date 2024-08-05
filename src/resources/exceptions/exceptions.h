#pragma once
#include <string>
#include "database_exception.h"

// Exceção para resultados vazios
class EmptyResultException : public DatabaseException {
public:
    explicit EmptyResultException(const std::string& query)
        : DatabaseException("Query returned no results: " + query) {}
};

// Exceção para erros de conexão
class ConnectionException : public DatabaseException {
public:
    explicit ConnectionException(const std::string& details)
        : DatabaseException("Database connection error: " + details) {}
};

// Exceção para erros de execução de consulta
class QueryExecutionException : public DatabaseException {
public:
    explicit QueryExecutionException(const std::string& query, const std::string& details)
        : DatabaseException("Error executing query: " + query + ". Details: " + details) {}
};