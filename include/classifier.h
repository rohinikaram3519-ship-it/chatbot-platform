#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <string>

class Classifier {
public:
    Classifier();
    std::string classify(const std::string& message);
    std::string generateResponse(const std::string& category);
};

#endif
