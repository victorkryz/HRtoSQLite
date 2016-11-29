cmake_minimum_required(VERSION 3.5)
project(HRtoSQLite_Root)

include_directories(HRtoSQLite HRtoSQLite/src)

add_subdirectory(HRtoSQLite)
add_subdirectory(HRtoSQLite_run)
add_subdirectory(HRtoSQLite_tests)





