cmake_minimum_required(VERSION 3.17)
project(learn_open_gl)

find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME} lib/glad.c main.cpp src/Shader.cpp lib/stb_image.cpp src/Texture.cpp)

target_link_libraries(${PROJECT_NAME} GL dl glfw)