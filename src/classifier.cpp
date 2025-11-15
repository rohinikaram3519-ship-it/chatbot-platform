#include "classifier.h"
#include <algorithm>

Classifier::Classifier() {
}

std::string Classifier::classify(const std::string& message) {
    std::string lower_msg = message;
    std::transform(lower_msg.begin(), lower_msg.end(), lower_msg.begin(), ::tolower);
    
    // Simple rule-based classification
    if (lower_msg.find("help") != std::string::npos || 
        lower_msg.find("support") != std::string::npos ||
        lower_msg.find("issue") != std::string::npos) {
        return "SUPPORT";
    } 
    else if (lower_msg.find("price") != std::string::npos || 
             lower_msg.find("cost") != std::string::npos ||
             lower_msg.find("pay") != std::string::npos) {
        return "PRICING";
    } 
    else if (lower_msg.find("hello") != std::string::npos || 
             lower_msg.find("hi") != std::string::npos ||
             lower_msg.find("hey") != std::string::npos) {
        return "GREETING";
    } 
    else {
        return "OTHER";
    }
}

std::string Classifier::generateResponse(const std::string& category) {
    if (category == "SUPPORT") {
        return "Thank you for reaching out! Our support team will help you shortly.";
    } 
    else if (category == "PRICING") {
        return "For pricing information, please visit our website or contact sales@example.com";
    } 
    else if (category == "GREETING") {
        return "Hello! Welcome to our chatbot. How can I help you today?";
    } 
    else {
        return "Thanks for your message. I'm here to help!";
    }
}
