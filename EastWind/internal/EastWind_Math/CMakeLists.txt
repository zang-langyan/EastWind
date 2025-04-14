cmake_minimum_required(VERSION 3.20)
if (WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU") 
project(EW_Math Fortran C)
else()
project(EW_Math)
endif()
set(CMAKE_CXX_STANDARD 17)

IF(NOT MSVC)
####################################################
#[[ BLAS ]]
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/BLAS)
####################################################

####################################################
#[[ LAPACK ]]
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/LAPACK)
####################################################
ENDIF()


####################################################
#[[ EastWind Math ]]
set(EW_MATH_DIR ${PROJECT_SOURCE_DIR})
file(GLOB_RECURSE EW_MATH_SRC ${EW_MATH_DIR}/src/*.cpp)
include_directories(${EW_MATH_DIR}/src)
include_directories(${EW_MATH_DIR})
# include_directories(${EW_MATH_DIR}/lib)

IF(MSVC)
	message("Building with MSVC: Using Intel OneAPI MKL as Math Library")
	# add_compile_definitions("EWMATH_MKL")
    # add_compile_definitions("MKL_INTERFACE_FULL=intel_lp64")
ENDIF()

IF(MSVC)
add_library(${PROJECT_NAME} STATIC ${EW_MATH_SRC})
ELSE()
add_library(${PROJECT_NAME} STATIC ${EW_MATH_SRC})
ENDIF()
# set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

IF(MSVC)
	if(WIN32)
		set(TEST_COPT "-D_CRT_SECURE_NO_WARNINGS")
	endif()
	find_package(MKL CONFIG REQUIRED PATHS $ENV{MKLROOT})
	target_include_directories(${PROJECT_NAME} PUBLIC $<TARGET_PROPERTY:MKL::MKL,INTERFACE_INCLUDE_DIRECTORIES>)
	target_link_libraries(${PROJECT_NAME} PUBLIC MKL::MKL)

	# target_link_libraries(${PROJECT_NAME} PUBLIC blas)
	# target_link_libraries(${PROJECT_NAME} PUBLIC lapack)
ELSE()
	# find_library(GFORTRAN_LIB libgfortran.a)
	# find_library(QUADMATH_LIB libquadmath.a)
	# find_library(GCC_LIB libgcc.a)
	# target_link_libraries(${PROJECT_NAME} PUBLIC ${GFORTRAN_LIB})
	# target_link_libraries(${PROJECT_NAME} PUBLIC ${QUADMATH_LIB})
	# target_link_libraries(${PROJECT_NAME} PUBLIC ${GCC_LIB})
	# target_link_libraries(${PROJECT_NAME} PUBLIC blas)
	target_link_libraries(${PROJECT_NAME} PUBLIC lapack)
ENDIF()

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
####################################################

####################################################
#[[ Test ]]
set(COMPILE_TEST OFF)
if(COMPILE_TEST)
  add_subdirectory(${PROJECT_SOURCE_DIR}/test)
endif()
####################################################


