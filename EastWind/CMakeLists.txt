cmake_minimum_required(VERSION 3.20)

project(EW)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE EW_SRC ${PROJECT_SOURCE_DIR}/src/*cpp)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/Event)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

add_library(EastWind STATIC ${EW_SRC})

target_precompile_headers(EastWind PUBLIC ${PROJECT_SOURCE_DIR}/src/EW_pch.h)

#[[ EastWind Math ]]
set(EastWind_MATH_DIR ${PROJECT_SOURCE_DIR}/internal/EastWind_Math)
target_include_directories(EastWind PUBLIC ${EastWind_MATH_DIR})
target_include_directories(EastWind PUBLIC ${EastWind_MATH_DIR}/src)
add_subdirectory(${EastWind_MATH_DIR})
target_link_libraries(EastWind EW_Math)

#[[ GLFW ]]
set(GLFW_DIR ${PROJECT_SOURCE_DIR}/vendor/glfw)
target_include_directories(EastWind PUBLIC ${GLFW_DIR}/include)
add_subdirectory(${GLFW_DIR})
add_compile_definitions(GLFW_INCLUDE_NONE)
target_link_libraries(EastWind glfw)

#[[ OpenGL ]]
find_package(OpenGL REQUIRED)
target_link_libraries(EastWind ${OPENGL_LIBRARIES})

#[[ Glad ]]
set(GLAD_DIR ${PROJECT_SOURCE_DIR}/vendor/glad)
target_include_directories(EastWind PUBLIC ${GLAD_DIR}/include)
add_subdirectory(${GLAD_DIR})
target_link_libraries(EastWind glad)

#[[ stb_image ]]
set(STB_IMAGE_DIR ${PROJECT_SOURCE_DIR}/vendor/stb_image)
target_include_directories(EastWind PUBLIC ${STB_IMAGE_DIR})
add_subdirectory(${STB_IMAGE_DIR})
target_link_libraries(EastWind stb_image)


# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
