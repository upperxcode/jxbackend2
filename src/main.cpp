#include "database/databaseSingleto.h"
#include "database/postgresDb.h"
#include <crow.h>

#include <src/routes/route>
#include "utils/print.h"
#include <dotenv.h>
#include <cstdlib>

int main() {
    // Carrega as variáveis de ambiente do arquivo .env
    dotenv::init();

    // Obtém as variáveis de ambiente
    const char* dbName = std::getenv("DB_NAME");
    const char* dbUser = std::getenv("DB_USER");
    const char* dbPassword = std::getenv("DB_PASSWORD");
    const char* dbHost = std::getenv("DB_HOST");
    const char* serverPortStr = std::getenv("SERVER_PORT");

    if (!dbName || !dbUser || !dbPassword || !dbHost || !serverPortStr) {
        console::error("Error: Missing environment variables.");
        return 1;
    }

    int serverPort = std::stoi(serverPortStr);

    // Conecta ao banco de dados PostgreSQL
    std::string connStr = "dbname=" + std::string(dbName) +
                          " user=" + std::string(dbUser) +
                          " password=" + std::string(dbPassword) +
                          " host=" + std::string(dbHost);

    PostgresDatabase &conn = DatabaseSingleton::getInstance(connStr);

    console::info("connected to ", conn.dbName(), " on ", dbHost);


    crow::SimpleApp app;
    
    route::produto::init(app);
    route::cas::init(app);
    route::dcb::init(app);
  

    app.port(serverPort).multithreaded().run();

   

    return 0;
}