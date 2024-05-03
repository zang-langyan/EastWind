cmake_minimum_required(VERSION 3.20)

project(sandbox)
set(CMAKE_CXX_STANDARD 17)

set(
    SANDBOX_SRC 
    ${PROJECT_SOURCE_DIR}/src/main.cpp
   )

include_directories(${PROJECT_SOURCE_DIR})



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
add_compile_definitions("STUDIO_HDRi_IMG=\"${PROJECT_SOURCE_DIR}/../EastWind/assets/textures/poly_haven_studio_4k.hdr\"")

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


set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/debug)

# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
