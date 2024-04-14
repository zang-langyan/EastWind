cmake_minimum_required(VERSION 3.20)
project(EW_Engine)

set(EW_DIR ${CMAKE_SOURCE_DIR}/EastWind)
set(Sandbox_DIR ${CMAKE_SOURCE_DIR}/Sandbox)


add_subdirectory(${EW_DIR})
add_subdirectory(${Sandbox_DIR})
