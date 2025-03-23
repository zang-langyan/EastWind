cmake_minimum_required(VERSION 3.20)

project(EW)
set(CMAKE_CXX_STANDARD 17)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  add_compile_definitions("EW_PLATFORM_OSX")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  add_compile_definitions("EW_PLATFORM_WINDOWS")
endif()


file(GLOB_RECURSE EW_SRC ${PROJECT_SOURCE_DIR}/src/*cpp)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/Event)

# add_compile_definitions("BASIC_SHADER_GLSL=\"${PROJECT_SOURCE_DIR}/assets/shaders/basic_shader.glsl\"")
# add_compile_definitions("BASIC_TEXTURE_SHADER_GLSL=\"${PROJECT_SOURCE_DIR}/assets/shaders/basic_texture_shader.glsl\"")
# add_compile_definitions("STUDIO_HDRi_IMG=\"${PROJECT_SOURCE_DIR}/assets/textures/poly_haven_studio_4k.hdr\"")

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

add_library(EastWind STATIC ${EW_SRC})
target_compile_definitions(EastWind 
  PUBLIC "BASIC_SHADER_GLSL=\"${PROJECT_SOURCE_DIR}/assets/shaders/basic_shader.glsl\""
  PUBLIC "BASIC_TEXTURE_SHADER_GLSL=\"${PROJECT_SOURCE_DIR}/assets/shaders/basic_texture_shader.glsl\""
  PUBLIC "STUDIO_HDRi_IMG=\"${PROJECT_SOURCE_DIR}/assets/textures/poly_haven_studio_4k.hdr\""
)

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

#[[ assimp ]]
set(ASSIMP_DIR ${PROJECT_SOURCE_DIR}/vendor/assimp)
target_include_directories(EastWind PUBLIC ${ASSIMP_DIR}/include)
add_subdirectory(${ASSIMP_DIR})
target_link_libraries(EastWind assimp)

#[[ bullet3 ]]
set(BULLET3_DIR ${PROJECT_SOURCE_DIR}/vendor/bullet3)
target_include_directories(EastWind PUBLIC ${BULLET3_DIR}/src)
include(${BULLET3_DIR}/UseBullet.cmake)
add_subdirectory(${BULLET3_DIR})
target_link_libraries(EastWind BulletDynamics BulletCollision LinearMath)

#[[ gflags ]]
set(GFLAGS_DIR ${PROJECT_SOURCE_DIR}/vendor/gflags)
target_include_directories(EastWind PUBLIC ${GFLAGS_DIR}/src)
add_subdirectory(${GFLAGS_DIR})
target_link_libraries(EastWind gflags::gflags)



# ---------------------------------------
# Unit Test
# ---------------------------------------
option(BUILD_UT "Build Unit Test" ON)
set(UT_DIR ${PROJECT_SOURCE_DIR}/ut)
if(BUILD_SINGLE)
  add_subdirectory(${UT_DIR})
endif()