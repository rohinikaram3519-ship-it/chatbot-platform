#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <sqlite3.h>
#include <crow.h>

class Database {
public:
    Database(const std::string& db_name);
    ~Database();
    
    void logMessage(const std::string& user_id, 
                   const std::string& message,
                   const std::string& response,
                   const std::string& category);
    
    std::vector<crow::json::wvalue> getAllLogs();
    crow::json::wvalue getStats();
    
private:
    sqlite3* db;
    void createTable();
};

#endif 