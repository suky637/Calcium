cmake_minimum_required(VERSION 3.5)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

project(compiler_settings)

add_library(compiler_settings src/compiler_settings.cpp)

set_property(TARGET compiler_settings PROPERTY CXX_STANDARD 17)

target_include_directories(compiler_settings PUBLIC "include/")
target_link_libraries(compiler_settings)