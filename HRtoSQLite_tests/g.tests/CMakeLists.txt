
#set(CMAKE_VERBOSE_MAKEFILE on)

set(VENDOR_DIR  "/home/victor/wp/ClionProjects/vendor")

set(CURRENT_DIR      "${CMAKE_CURRENT_SOURCE_DIR}")
set(SOURCE_FILES_DIR "${CURRENT_DIR}/src")

# set Google test pathes:
set(GTEST_INC_DIR  "${VENDOR_DIR}/googletest-1.7.0/googletest/include")
set(GTEST_LIB_DIR  "${VENDOR_DIR}/googletest-1.7.0/googletest")


set(GTEST_LIBS ${GTEST_LIB_DIR}/libgtest.a #[[${GTEST_LIB_DIR}/libgtest_main.a]])

add_definitions(-D_GTEST_)

aux_source_directory("${SOURCE_FILES_DIR}" Source_files)
aux_source_directory("${SOURCE_FILES_DIR}/Ora" Source_files_Ora)
aux_source_directory("${SOURCE_FILES_DIR}/Sqlt" Source_files_Sqlt)
aux_source_directory("${SOURCE_FILES_DIR}/Others" Source_files_Others)

add_executable(GTests_run   ${Source_files}
                            ${Source_files_Ora}
                            ${Source_files_Sqlt}
                            ${Source_files_Others})


target_compile_options(GTests_run PUBLIC -g -std=c++1y)

set_target_properties(GTests_run PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(GTests_run PRIVATE ${GTEST_INC_DIR})

target_link_libraries(GTests_run PUBLIC ${GTEST_LIBS})
target_link_libraries(GTests_run PUBLIC HRtoSQLite -lpthread -lm)
