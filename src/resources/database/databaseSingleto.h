// DatabaseSingleton.h
#pragma once

#include "postgresDb.h"
#include <memory>
#include <mutex>
#include <stdexcept>

class DatabaseSingleton {
public:
    static PostgresDatabase& getInstance(const std::string& connectionString = "") {
        if (instance) {
            if (!connectionString.empty()) {
                throw std::runtime_error("DatabaseSingleton already initialized. Cannot reinitialize with a new connection string.");
            }
        } else {
            std::call_once(initInstanceFlag, &DatabaseSingleton::initSingleton, connectionString);
        }

        if (!instance) {
            throw std::runtime_error("DatabaseSingleton not initialized. Call getInstance with a connection string.");
        }
        return *instance;
    }

    // Delete copy constructor and assignment operator
    DatabaseSingleton(const DatabaseSingleton&) = delete;
    DatabaseSingleton& operator=(const DatabaseSingleton&) = delete;

private:
    DatabaseSingleton() = default;
    ~DatabaseSingleton() = default;

    static void initSingleton(const std::string& connectionString) {
        if (connectionString.empty()) {
            throw std::invalid_argument("Connection string cannot be empty on first call.");
        }
        instance.reset(new PostgresDatabase(connectionString));
    }

    static std::unique_ptr<PostgresDatabase> instance;
    static std::once_flag initInstanceFlag;
};

// Definition of static members
std::unique_ptr<PostgresDatabase> DatabaseSingleton::instance;
std::once_flag DatabaseSingleton::initInstanceFlag;