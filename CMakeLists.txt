cmake_minimum_required(VERSION 3.15)
project(ChatbotPlatform)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(Threads REQUIRED)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
    src/classifier.cpp
    src/database.cpp
)

# Create executable
add_executable(chatbot_server ${SOURCES})

# Link libraries
target_link_libraries(chatbot_server
    Threads::Threads
    sqlite3
    curl
)
