cmake_minimum_required(VERSION 3.20)
project(PyEastWind)

set(PYBIND11_DIR ${PROJECT_SOURCE_DIR}/extern/pybind11)
set(IMGUI_DIR ${PROJECT_SOURCE_DIR}/../Sandbox/vendor/imgui)

add_subdirectory(${PYBIND11_DIR})

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE EW_PY_SRC ${PROJECT_SOURCE_DIR}/src/*.cpp)
list(APPEND EW_PY_SRC 
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    )

include_directories(${PYBIND11_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)

pybind11_add_module(${PROJECT_NAME} ${EW_PY_SRC})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME EastWind)

target_include_directories(${PROJECT_NAME} PUBLIC ${EW_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${EW_DIR}/src)
target_link_libraries(${PROJECT_NAME} PUBLIC EastWind)

add_compile_definitions("IMGUI_FONT_PATH=\"${PROJECT_SOURCE_DIR}/../Sandbox/assets/fonts/Noto_Sans_SC/NotoSansSC-Bold.otf\"")
target_include_directories(${PROJECT_NAME} PUBLIC ${IMGUI_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${IMGUI_DIR}/backends)
target_link_libraries(${PROJECT_NAME} PUBLIC imgui)