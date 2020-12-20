cmake_minimum_required(VERSION 3.8)

project(plasma)

set(SOURCE main.cpp scene.cpp)

add_executable(${PROJECT_NAME} ${SOURCE})

set(CMAKE_CXX_COMPILER ${CMAKE_CXX_COMPILER} CACHE FILEPATH "Compiler")
set(CMAKE_C_COMPILER ${CMAKE_C_COMPILER} CACHE FILEPATH "Compiler")