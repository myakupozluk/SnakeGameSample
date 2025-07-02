cmake_minimum_required(VERSION 3.31)
project(SnakeGame)

set(CMAKE_CXX_STANDARD 20)

set(PROJECT_DIR ${CMAKE_SOURCE_DIR})
set(PROJECT_INCLUDE_DIR ${PROJECT_DIR}/include)
set(PROJECT_SOURCE_DIR ${PROJECT_DIR}/src)

find_package(glfw3 REQUIRED)

add_executable(SnakeGame
        ${PROJECT_SOURCE_DIR}/main.cpp
        ${PROJECT_SOURCE_DIR}/glad.c
        ${PROJECT_SOURCE_DIR}/shaderProgram.cpp
        ${PROJECT_SOURCE_DIR}/square.cpp
)

target_include_directories(SnakeGame PRIVATE
        ${PROJECT_INCLUDE_DIR}
        ${PROJECT_INCLUDE_DIR}/KHR
        ${PROJECT_INCLUDE_DIR}/square.hpp
        ${PROJECT_INCLUDE_DIR}/shaderProgram.hpp
        /usr/include
)

target_link_libraries(SnakeGame PRIVATE glfw GL)
