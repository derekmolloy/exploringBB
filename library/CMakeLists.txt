cmake_minimum_required(VERSION 2.8)

project (test)
set(CMAKE_BUILD_TYPE Release)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/gpio)
add_library(test SHARED src/test.cpp)
