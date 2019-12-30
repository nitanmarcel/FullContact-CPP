cmake_minimum_required(VERSION 3.10)
project(FullContact_CPP)

set(CMAKE_CXX_STANDARD 14)

find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

add_executable(FullContact_CPP main.cpp src/fullcontact.cpp)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json openssl curl)
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json curl)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE include src)

