cmake_minimum_required(VERSION 3.13)

project(multiway_cut)

set(MAIN_SOURCE main.cpp)
set(CHECK_CUT_SOURCE check_cut.cpp)
set(SOURCE_LIB graph.cpp)

add_library(graph STATIC ${SOURCE_LIB})

add_executable(main ${MAIN_SOURCE})
add_executable(check_cut ${CHECK_CUT_SOURCE})

target_link_libraries(main graph)
target_link_libraries(check_cut graph)