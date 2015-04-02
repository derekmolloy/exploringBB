cmake_minimum_required(VERSION 2.8.9)
project (TestLibrary)
set ( PROJECT_LINK_LIBS libtestStudent.so )
#link_directories( /home/molloyd/exploringBB/extras/cmake/studentlib/build )
link_directories( ~/exploringBB/extras/cmake/studentlib/build )

include_directories(~/exploringBB/extras/cmake/studentlib/include)

add_executable(libtest libtest.cpp)
target_link_libraries(libtest ${PROJECT_LINK_LIBS} )

