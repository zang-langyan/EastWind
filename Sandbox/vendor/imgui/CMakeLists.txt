cmake_minimum_required(VERSION 3.20)

project(imgui)
set(CMAKE_CXX_STANDARD 17)


set(IMGUI_SRC
    ${PROJECT_SOURCE_DIR}/imgui.cpp
    ${PROJECT_SOURCE_DIR}/imgui_demo.cpp
    ${PROJECT_SOURCE_DIR}/imgui_draw.cpp
    ${PROJECT_SOURCE_DIR}/imgui_tables.cpp
    ${PROJECT_SOURCE_DIR}/imgui_widgets.cpp
    )

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/backends)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

add_library(${PROJECT_NAME} STATIC ${IMGUI_SRC})

# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
