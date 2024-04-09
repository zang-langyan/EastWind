cmake_minimum_required(VERSION 3.20)

project(stb_image)
set(CMAKE_CXX_STANDARD 17)


set(STB_IMAGE_SRC ${PROJECT_SOURCE_DIR}/stb_image.cpp)

include_directories(${PROJECT_SOURCE_DIR})

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

add_library(${PROJECT_NAME} STATIC ${STB_IMAGE_SRC})

