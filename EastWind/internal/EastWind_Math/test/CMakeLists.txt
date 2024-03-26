cmake_minimum_required(VERSION 3.20)

project(EW_MATH_TEST)
set(CMAKE_CXX_STANDARD 17)


#[[ EastWind Math Library Blas Test ]]
add_executable(EW_BLAS_TEST ${PROJECT_SOURCE_DIR}/EW_MATH_TEST.cpp)
target_include_directories(EW_BLAS_TEST PUBLIC ${EW_MATH_DIR}/src)
target_include_directories(EW_BLAS_TEST PUBLIC ${EW_MATH_DIR})
target_link_libraries(EW_BLAS_TEST EW_Math)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/test)


