#include <iostream>
#include <string>
#include <vector>
#include "httplib.h"
#include "database.h"
#include "classifier.h"

int main() {
    httplib::Server svr;
    
    Database db("chatbot.db");
    db.initialize();
    
    Classifier classifier;
    
    std::cout << "Initializing Chatbot Server..." << std::endl;

    // Health check endpoint
    svr.Get("/health", [](const httplib::Request&, httplib::Response& res) {
        res.set_content(R"({"status": "healthy", "message": "Chatbot is running"})", "application/json");
    });

    // Root endpoint
    svr.Get("/", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("Chatbot Server is Running!", "text/plain");
    });

    // Receive message endpoint
    svr.Post("/message", [&db, &classifier](const httplib::Request& req, httplib::Response& res) {
        std::string user_id = req.get_param_value("user_id");
        std::string message = req.get_param_value("message");
        
        if (user_id.empty() || message.empty()) {
            res.status = 400;
            res.set_content(R"({"error": "user_id and message are required"})", "application/json");
            return;
        }
        
        // Classify the message
        std::string category = classifier.classify(message);
        
        // Generate response based on category
        std::string response = classifier.generateResponse(category);
        
        // Log to database
        db.logMessage(user_id, message, response, category);
        
        // Build JSON response
        std::string json_response = R"({"response": ")" + response + R"(", "category": ")" + category + R"("})";
        res.set_content(json_response, "application/json");
    });

    // Get all logs
    svr.Get("/logs", [&db](const httplib::Request&, httplib::Response& res) {
        std::string logs = db.getAllLogsJSON();
        res.set_content(logs, "application/json");
    });

    // Get statistics
    svr.Get("/stats", [&db](const httplib::Request&, httplib::Response& res) {
        std::string stats = db.getStatsJSON();
        res.set_content(stats, "application/json");
    });

    std::cout << "Starting chatbot server on port 8080..." << std::endl;
    std::cout << "Server running at http://0.0.0.0:8080" << std::endl;
    
    svr.listen("0.0.0.0", 8080);
    
    return 0;
}
