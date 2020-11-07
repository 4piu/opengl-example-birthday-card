cmake_minimum_required(VERSION 3.17)
project(birthday_card)

set(CMAKE_CXX_STANDARD 14)

set(OpenGlLinkers -lGLEW -lglut -lGLU -lGL)

add_executable(birthday_card main.cpp utilities.cpp)

target_link_libraries(birthday_card ${OpenGlLinkers})