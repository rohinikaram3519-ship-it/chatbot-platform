#include "database.h"
#include <iostream>
#include <sstream>
#include <sqlite3.h>
#include <ctime>

Database::Database(const std::string& path) : db_path(path) {
}

void Database::initialize() {
    sqlite3 *db;
    int rc = sqlite3_open(db_path.c_str(), &db);
    
    if (rc) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }
    
    const char* sql = "CREATE TABLE IF NOT EXISTS messages("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "user_id TEXT,"
                      "message TEXT,"
                      "response TEXT,"
                      "category TEXT,"
                      "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP);";
    
    char* err_msg = 0;
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << err_msg << std::endl;
        sqlite3_free(err_msg);
    } else {
        std::cout << "Database initialized successfully" << std::endl;
    }
    
    sqlite3_close(db);
}

void Database::logMessage(const std::string& user_id, const std::string& message, 
                          const std::string& response, const std::string& category) {
    sqlite3 *db;
    sqlite3_open(db_path.c_str(), &db);
    
    std::string sql = "INSERT INTO messages(user_id, message, response, category) VALUES(?, ?, ?, ?);";
    sqlite3_stmt *stmt;
    
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) != SQLITE_OK) {
        std::cerr << "SQL prepare error" << std::endl;
        sqlite3_close(db);
        return;
    }
    
    sqlite3_bind_text(stmt, 1, user_id.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, message.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, response.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, category.c_str(), -1, SQLITE_STATIC);
    
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

std::vector<LogEntry> Database::getAllLogs() {
    std::vector<LogEntry> logs;
    sqlite3 *db;
    sqlite3_stmt *stmt;
    
    sqlite3_open(db_path.c_str(), &db);
    
    const char* sql = "SELECT id, user_id, message, response, category, timestamp FROM messages ORDER BY id DESC LIMIT 100;";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        std::cerr << "SQL select error" << std::endl;
        sqlite3_close(db);
        return logs;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        LogEntry entry;
        entry.id = sqlite3_column_int(stmt, 0);
        entry.user_id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        entry.message = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        entry.response = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        entry.category = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        entry.timestamp = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        logs.push_back(entry);
    }
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return logs;
}

std::string Database::getAllLogsJSON() {
    auto logs = getAllLogs();
    std::stringstream ss;
    
    ss << R"({"logs": [)";
    for (size_t i = 0; i < logs.size(); ++i) {
        ss << R"({"id": )" << logs[i].id 
           << R"(, "user_id": ")" << logs[i].user_id 
           << R"(", "message": ")" << logs[i].message 
           << R"(", "response": ")" << logs[i].response 
           << R"(", "category": ")" << logs[i].category 
           << R"(", "timestamp": ")" << logs[i].timestamp << R"("})";
        
        if (i < logs.size() - 1) ss << ",";
    }
    ss << R"(]})";
    
    return ss.str();
}

std::string Database::getStatsJSON() {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    sqlite3_open(db_path.c_str(), &db);
    
    std::stringstream ss;
    
    // Total messages
    const char* sql = "SELECT COUNT(*), COUNT(DISTINCT user_id) FROM messages;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int total = sqlite3_column_int(stmt, 0);
            int users = sqlite3_column_int(stmt, 1);
            
            ss << R"({"total_messages": )" << total 
               << R"(, "total_users": )" << users << "}";
        }
        sqlite3_finalize(stmt);
    }
    
    sqlite3_close(db);
    return ss.str();
}
