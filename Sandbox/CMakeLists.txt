cmake_minimum_required(VERSION 3.20)

project(sandbox)
set(CMAKE_CXX_STANDARD 17)

set(SANDBOX_SRC ${PROJECT_SOURCE_DIR}/src/SandBoxApp.cpp)

include_directories(${PROJECT_SOURCE_DIR})



#[[ ImGui ]]
set(IMGUI_DIR ${PROJECT_SOURCE_DIR}/vendor/debug/imgui)
add_subdirectory(${IMGUI_DIR})
list(APPEND SANDBOX_SRC 
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    )


add_compile_definitions("IMGUI_FONT_PATH=\"${PROJECT_SOURCE_DIR}/assets/Noto_Sans_SC/NotoSansSC-Bold.otf\"")


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
