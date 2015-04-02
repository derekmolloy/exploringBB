cmake_minimum_required(VERSION 2.8)
project(directory_test)
set(CMAKE_BUILD_TYPE Release)

#Bring the headers, such as Student.h into the project
include_directories(include)

#However, the file(GLOB...) will allow for wildcard additions:
file(GLOB SOURCES "src/*.cpp")

add_library(testStudent STATIC ${SOURCES})
