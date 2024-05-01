cmake_minimum_required(VERSION 3.20)

project(glad)
set(CMAKE_C_STANDARD 11)

set(GLAD_SRC ${PROJECT_SOURCE_DIR}/src/glad.c)

add_library(${PROJECT_NAME} STATIC ${GLAD_SRC})

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib/${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include/glad)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include/KHR)
