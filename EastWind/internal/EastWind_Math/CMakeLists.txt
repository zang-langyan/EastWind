cmake_minimum_required(VERSION 3.20)

project(EW_Math)
set(CMAKE_CXX_STANDARD 17)

####################################################
#[[ BLAS ]]
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/BLAS)
####################################################

####################################################
#[[ LAPACK ]]
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/LAPACK)
####################################################




####################################################
#[[ EastWind Math ]]
set(EW_MATH_DIR ${PROJECT_SOURCE_DIR})
file(GLOB_RECURSE EW_MATH_SRC ${EW_MATH_DIR}/src/*.cpp)
include_directories(${EW_MATH_DIR}/src)
include_directories(${EW_MATH_DIR})
# include_directories(${EW_MATH_DIR}/lib)

add_library(${PROJECT_NAME} STATIC ${EW_MATH_SRC})
# set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
find_library(GFORTRAN_LIB 
  NAME gfortran 
  HINTS ${PROJECT_SOURCE_DIR}/lib/gcc
  REQUIRED)
find_library(QUADMATH_LIB 
  NAME quadmath 
  HINTS ${PROJECT_SOURCE_DIR}/lib/gcc
  REQUIRED)
target_link_libraries(${PROJECT_NAME} ${GFORTRAN_LIB})
target_link_libraries(${PROJECT_NAME} ${QUADMATH_LIB})
target_link_libraries(${PROJECT_NAME} blas)
target_link_libraries(${PROJECT_NAME} lapack)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
####################################################

####################################################
#[[ Test ]]
set(COMPILE_TEST OFF)
if(COMPILE_TEST)
  add_subdirectory(${PROJECT_SOURCE_DIR}/test)
endif()
####################################################


