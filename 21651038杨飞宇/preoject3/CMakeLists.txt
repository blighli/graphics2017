cmake_minimum_required(VERSION 3.7)
project(SimpleSolarSystem)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Camera.h Shader.h Mesh.h Model.h Mesh.h Model.h)
add_executable(SimpleSolarSystem ${SOURCE_FILES})