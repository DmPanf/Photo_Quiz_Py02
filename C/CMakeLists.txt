cmake_minimum_required(VERSION 3.10)
project(quiz_bot)

set(CMAKE_CXX_STANDARD 17)

find_package(TgBot REQUIRED)

add_executable(quiz_bot main.cpp)
target_link_libraries(quiz_bot TgBot pthread ssl crypto)
