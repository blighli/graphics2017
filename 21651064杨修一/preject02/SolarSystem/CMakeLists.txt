cmake_minimum_required(VERSION 3.6)
project(SolarSystem)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES Camera.h main.cpp)
add_executable(SolarSystem ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} GL GLU glut GLEW glfw X11 Xi Xrandr Xxf86vm Xinerama Xcursor rt m pthread dl SOIL)