cmake_minimum_required(VERSION 3.17)
project(learn_open_gl)

find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME} glad.c main.cpp Shader.cpp stb_image.cpp)

target_link_libraries(${PROJECT_NAME} GL dl glfw)