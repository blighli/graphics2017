cmake_minimum_required(VERSION 3.7)

project(graphics01)

include_directories(${graphics01_SOURCE_DIR}/include)

set(APP_SRC ${graphics01_SOURCE_DIR}/src/graphics01.cpp)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
add_executable(graphics01 ${APP_SRC})

set(TARGET_LIB
               "${graphics01_SOURCE_DIR}/lib/libfreeglut.a"
               "${graphics01_SOURCE_DIR}/lib/libfreeglut_static.a"
               "${graphics01_SOURCE_DIR}/lib/GLU32.lib"
               "${graphics01_SOURCE_DIR}/lib/OpenGL32.lib")
target_link_libraries(graphics01 ${TARGET_LIB})

add_subdirectory(src)

add_definitions(-DDEBUG)