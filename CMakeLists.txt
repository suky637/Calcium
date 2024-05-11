cmake_minimum_required(VERSION 3.5)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

add_subdirectory(compiler)

project(Calcium)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

add_executable(Calcium src/main.cpp)

set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY CXX_STANDARD 17)

target_include_directories(Calcium PUBLIC "include/")
target_link_libraries(Calcium compiler)