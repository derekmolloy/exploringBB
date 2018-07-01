cmake_minimum_required(VERSION 3.0.2)
project (TestEBBLibrary)
 
#For the shared library:
set ( PROJECT_LINK_LIBS libEBBLibrary.so )
link_directories( ~/exploringbb/library/build )
 
#For the static library:
#set ( PROJECT_LINK_LIBS libEBBLibrary.a )
#link_directories( ~/exploringbb/library/build )
 
include_directories(~/exploringbb/library/)
 
add_executable(libtest libtest.cpp)
target_link_libraries(libtest ${PROJECT_LINK_LIBS} )
