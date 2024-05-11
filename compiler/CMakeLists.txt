cmake_minimum_required(VERSION 3.5)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

project(compiler)

add_library(compiler src/lexer.cpp src/parser.cpp)

set_property(TARGET compiler PROPERTY CXX_STANDARD 17)

target_include_directories(compiler PUBLIC "include/")
target_link_libraries(compiler)