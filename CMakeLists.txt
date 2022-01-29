cmake_minimum_required(VERSION 3.8)

project(learnopengl)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
add_library(GLAD "./glad.c")
find_package(glfw3 REQUIRED)

add_executable(${PROJECT_NAME} main.cpp glad.c) #The name of the cpp file and its path can vary
target_link_libraries(${PROJECT_NAME} OpenGL::GL glfw dl GLAD)