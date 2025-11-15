#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>

struct LogEntry {
    int id;
    std::string user_id;
    std::string message;
    std::string response;
    std::string category;
    std::string timestamp;
};

class Database {
private:
    std::string db_path;
public:
    Database(const std::string& path);
    void initialize();
    void logMessage(const std::string& user_id, const std::string& message, 
                    const std::string& response, const std::string& category);
    std::vector<LogEntry> getAllLogs();
    std::string getAllLogsJSON();
    std::string getStatsJSON();
};

#endif
