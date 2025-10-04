cmake_minimum_required(VERSION 3.20)

project(sandbox)
set(CMAKE_CXX_STANDARD 17)

set(
    SANDBOX_SRC 
    ${PROJECT_SOURCE_DIR}/src/main.cpp
)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/src)

# [[ Yaml ]]
set(YAML_DIR ${PROJECT_SOURCE_DIR}/vendor/yaml-cpp)
add_subdirectory(${YAML_DIR})

#[[ ImGui ]]
set(IMGUI_DIR ${PROJECT_SOURCE_DIR}/vendor/imgui)
add_subdirectory(${IMGUI_DIR})
list(APPEND SANDBOX_SRC 
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    )

# Shaders
add_compile_definitions("TEXTURE_SHADER_PATH=\"${PROJECT_SOURCE_DIR}/assets/shaders/texture_shader.glsl\"")

# Fonts
add_compile_definitions("IMGUI_FONT_PATH=\"${PROJECT_SOURCE_DIR}/assets/fonts/Noto_Sans_SC/NotoSansSC-Bold.otf\"")

# Textures
add_compile_definitions("CHECKBOARD_TEXTURE_PATH=\"${PROJECT_SOURCE_DIR}/assets/textures/Checkerboard.png\"")
add_compile_definitions("RIVERROCK_TEXTURE_PATH=\"${PROJECT_SOURCE_DIR}/assets/textures/Dry_RiverBed_Rock/dry_riverbed_rock_diff_1k.png\"")
add_compile_definitions("HAMMERSICKEL_TEXTURE_PATH=\"${PROJECT_SOURCE_DIR}/assets/textures/Hammer_Sickel/Hammer_Sickel.png\"")
# add_compile_definitions("STUDIO_HDRi_IMG=\"${PROJECT_SOURCE_DIR}/../EastWind/assets/textures/poly_haven_studio_4k.hdr\"")

# Meshes
add_compile_definitions("RABBIT_OFF_FILE_PATH=\"${PROJECT_SOURCE_DIR}/assets/meshes/bunny_nf4000.noff\"")

add_executable(${PROJECT_NAME} ${SANDBOX_SRC})

#[[ EastWind ]]
target_include_directories(${PROJECT_NAME} PUBLIC ${EW_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${EW_DIR}/src)
target_link_libraries(${PROJECT_NAME} PUBLIC EastWind)

#[[ ImGui ]]
target_include_directories(${PROJECT_NAME} PUBLIC ${IMGUI_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${IMGUI_DIR}/backends)
target_link_libraries(${PROJECT_NAME} PUBLIC imgui)

#[[ YAML ]]
target_include_directories(${PROJECT_NAME} PUBLIC ${YAML_DIR}/include)
target_link_libraries(${PROJECT_NAME} PUBLIC yaml-cpp::yaml-cpp)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/debug)


# ===================================
# Configuration Folder
# ===================================
add_custom_target(copy_configurations
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${PROJECT_SOURCE_DIR}/config ${CMAKE_CURRENT_BINARY_DIR}/config
)
add_dependencies(${PROJECT_NAME} copy_configurations)


# ===================================
# Asset Folder
# ===================================
add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${PROJECT_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)
add_dependencies(${PROJECT_NAME} copy_assets)

# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
