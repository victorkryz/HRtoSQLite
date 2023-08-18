
#set(CMAKE_VERBOSE_MAKEFILE on)

set(CURRENT_DIR      "${CMAKE_CURRENT_SOURCE_DIR}")
set(SOURCE_FILES_DIR "${CURRENT_DIR}/src")

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/f8d7d77c06936315286eb55f8de22cd23c188571.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

aux_source_directory("${SOURCE_FILES_DIR}" Source_files)
aux_source_directory("${SOURCE_FILES_DIR}/Ora" Source_files_Ora)
aux_source_directory("${SOURCE_FILES_DIR}/Sqlt" Source_files_Sqlt)
aux_source_directory("${SOURCE_FILES_DIR}/Others" Source_files_Others)

add_executable(GTests_run   ${Source_files}
                            ${Source_files_Ora}
                            ${Source_files_Sqlt}
                            ${Source_files_Others})

target_compile_options(GTests_run PUBLIC -g -std=c++17)

set_target_properties(GTests_run PROPERTIES LINKER_LANGUAGE CXX)


target_link_libraries(GTests_run PUBLIC GTest::gtest_main 
                                        HRtoSQLite 
                                        -lpthread -lm)
