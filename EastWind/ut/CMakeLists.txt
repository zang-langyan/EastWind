cmake_minimum_required(VERSION 3.20)

project(EW_unit_test)
set(CMAKE_CXX_STANDARD 17)

set(
  MD5_TEST_SRC 
  ${PROJECT_SOURCE_DIR}/md5_test.cpp
)

include_directories(${PROJECT_SOURCE_DIR})
add_executable(ew_md5sum ${MD5_TEST_SRC})

#[[ EastWind ]]
target_include_directories(ew_md5sum PUBLIC ${EW_DIR})
target_include_directories(ew_md5sum PUBLIC ${EW_DIR}/src)
target_link_libraries(ew_md5sum PUBLIC EastWind)
