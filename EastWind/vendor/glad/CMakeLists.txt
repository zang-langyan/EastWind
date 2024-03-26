cmake_minimum_required(VERSION 3.20)

project(glad)
set(CMAKE_C_STANDARD 11)

set(GLAD_SRC ${PROJECT_SOURCE_DIR}/src/glad.c)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/glad)
include_directories(${PROJECT_SOURCE_DIR}/include/KHR)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib/${PROJECT_NAME})

add_library(glad STATIC ${GLAD_SRC})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
