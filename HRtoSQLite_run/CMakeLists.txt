cmake_minimum_required(VERSION 3.5)
project(HRtoSQLite_run)

#set(CMAKE_VERBOSE_MAKEFILE on)

add_definitions(-std=c++11)

set(SOURCE_FILES ./src/main.cpp)
add_executable(HRtoSQLite_run "${SOURCE_FILES}")

target_link_libraries(HRtoSQLite_run HRtoSQLite)

