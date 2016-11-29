cmake_minimum_required(VERSION 3.5)
project(HRtoSQLite)

#set(CMAKE_VERBOSE_MAKEFILE on)
#------------------------------------------------------

set(VENDOR_DIR      "/home/victor/wp/ClionProjects/vendor")
set(CURRENT_DIR      "${CMAKE_CURRENT_SOURCE_DIR}")
set(SOURCE_FILES_DIR "${CURRENT_DIR}/src")

# set OCI and Poco libs paths:
#--------------------------------------------------------
set(OCI_INC_DIR      "/usr/include/oracle/12.1/client64")
set(OCI_LIB_DIR      "/usr/lib/oracle/12.1/client64/lib")
set(POCO_INC_DIR     "${VENDOR_DIR}/poco/include")
set(POCO_LIB_DIR     "${VENDOR_DIR}/poco/lib")
#---------------------------------------------------------


function(check_vars var)
    if(NOT DEFINED ${var} )
        message(FATAL_ERROR "${var} is not defined!")
     endif()
endfunction(check_vars)


check_vars(OCI_INC_DIR)
check_vars(OCI_LIB_DIR)
check_vars(POCO_INC_DIR)
check_vars(POCO_LIB_DIR)

include_directories("${SOURCE_FILES_DIR}")

aux_source_directory("${SOURCE_FILES_DIR}" HRtoSQLite_files)
aux_source_directory("${SOURCE_FILES_DIR}/Ora" Source_files_Ora)
aux_source_directory("${SOURCE_FILES_DIR}/Sqlt" Source_files_Sqlt)
add_library(HRtoSQLite STATIC ${HRtoSQLite_files} ${Source_files_Ora} ${Source_files_Sqlt})

target_compile_options(HRtoSQLite PUBLIC -g -std=c++1y)

set_target_properties(HRtoSQLite PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(HRtoSQLite PUBLIC ${OCI_INC_DIR} PUBLIC ${POCO_INC_DIR})

set(OCI_LIBS    ${OCI_LIB_DIR}/libclntsh.so)
set(POCO_LIBS   ${POCO_LIB_DIR}/libPocoFoundation
                ${POCO_LIB_DIR}/libPocoUtil
                ${POCO_LIB_DIR}/libPocoData
                ${POCO_LIB_DIR}/libPocoDataSQLite)

set (POCO_LINKED_LIBS "")
foreach(x ${POCO_LIBS})
    list (APPEND POCO_LINKED_LIBS debug ${x}d.so optimized ${x}.so)
endforeach()

target_link_libraries(HRtoSQLite   PUBLIC ${OCI_LIBS}
                                   PUBLIC ${POCO_LINKED_LIBS}
                                   pthread)
