# HRtoSQLite
HRtoSQLite utility transfers Oracle Database sample HR schema's data into SQLite database.


#### Motivations:

- Get acquainted with the POCO C++. 
- Using POCO data access API for operating on SQLite database.
- Make it cross-platform (Windows/Linux OS).
 
 
#### Features:
 
 - Oracle call interface (OCI) is used to Oracle database access;
 - Poco features:
    - there's used POCO-proposed skeleton for application organizing (Application class); 
    - data access API (SQLite database);
    - command-line parameters handling feature;
    - logging into application log;
    - some utilities (e.g. string formatting);
 - "Google's C++ test" as a unit testing framework.
 - CMake based configuration;


### Usage:
-------------------------------------------------------------------------

#### Command line arguments:

```
-h, --help                      display help information on command line arguments  
-C value, --ora-conn-str=value  Oracle connection string   (//host[:port]/service)  
-N value, --ora-name=value      Oracle user name (default: "hr")  
-P value, --ora-psw=value       Oracle user password  
-S file, --sqlt-ddl=file        HR-SQLite database structute intialization file  
                                (default: ./sql/hrSQLiteDDL.sql)  
-O file, --output=file          SQLite database file (default: ./hr.db)  
```

Connection string format confirms to  [*Database Connection Strings for OCI Instant Client*](http://docs.oracle.com/database/121/LNOCI/oci01int.htm#LNOCI16167)

Command line samples:

```
HRtoSQLite_run -C//oracle_host/xe -N hr -P hr -S./../sql/hrSQLiteDDL.sql
HRtoSQLite_run -C//oracle_host/xe -N hr -P hr -S./../sql/hrSQLiteDDL.sql -O./../dbs/hr.db
```

### Building:
-------------------------------------------------------------------------

- Download and install [*Oracle Instant Client (Package - SDK)*](http://www.oracle.com/technetwork/database/features/instant-client/index-097480.html)
- Download and build [*Poco C++ libraries*](https://pocoproject.org/download/)
- Download and build [*Google's C++ test framework*](https://github.com/google/googletest) ([*read build instructions*](https://github.com/google/googletest/blob/master/googletest/README.md)) 

 *(On the development phase, there's used POCO version 1.7.4, Oracle client 12.1, googletest-1.14.0)*

- #### For linux:

    The project building is based on CMake configuration files.   
    Set the next variables that define headers and libraries directories:  
    
    - go into file HRtoSQLite/CMakeLists.txt and set OCI and Poco paths in variables:  
      *OCI_INC_DIR, OCI_LIB_DIR, POCO_INC_DIR, POCO_LIB_DIR*;
    - go into file HRtoSQLite_tests/g.tests/CMakeLists.txt and set google test paths in variables:   
      *GTEST_INC_DIR, GTEST_LIB_DIR*

       
    *To build it without IDE, using only CMake CLI, do steps like below:*  

        ```
        mkdir build-debug
        cd build-debug
        cmake -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - Unix Makefiles" ../.
        cmake --build ./ --target all
        ```

- #### For Windows (Visual Studio 2015):

    -go into directory HRtoSQLite.VS2015 and open HRToSQLiteWS.sln solution;

        Projects sesttings suppose the next default headers and libraries directories configuration:   

        ..\..\..\vendor\instantclient_12_1\sdk\include  - OCI headers
        ..\..\..\vendor\poco-1.7.4\include              - POCO headers
        ..\..\..\vendor\instantclient_12_1\sdk\lib\msvc - OCI libraries
        .\..\..\vendor\poco-1.7.4\lib64                 - POCO libraries

    *(note: there's only x64 configuration provided)*

    Simulate a similar directories disposition (e.g. using file system links) or substitute these
    directories on yours in projects settings (*"Additional Include Directories"*, *"Additional Library Directories"*)

        To get "google test" framework integrated into Visual Studio IDE, install the next NuGet packages for 
        the project "HRtoSQLite_tests":
        - gtest 17.0
        - fix8.dependencies.gtest
        - fix8.dependencies.gtest.symbols
        


### Testing:
-------------------------------------------------------------------------

To get the unit tests runnable, build the subproject *HRtoSQLite_tests*.  
There are provided a set of tests for Oracle connection, reading 'HR' schema tables, writing data into SQLite database, e.g.

The tests are parametrized by environment variables:

```
ORA_CONN_STR   Oracle connection string
ORA_NAME       Oracle user name
ORA_PSW        Oracle user password 
SQLT_DDL       HR-SQLite database structure initialization file
OUTPUT         SQLite database file (default: ./hr.db)
```

Directory 'Launchers' contains some configuration and scripts to help test launching.  
To launch unit tests from command line, setup variables in *'config.env'* file and launch *'testLauncher'* script.

**Warning:** in case these tests are launched under from IDE (e.g.Visual Studio), setup the given environment 
variables in the respective configuration (debug/run) before debugging or running some test(s)!  


-------------------------------------------------------------------------
Project built and tested under OS:   
- *Windows 7 (64-bit) (Microsoft Visual Studio 2015 (Version 14.0.25123.00))* 
- *Ubuntu 16.04 LTS (64-bit) (g++ 5.4.0)*
- *Red Hat 7.2 (64-bit) (g++ 4.8.5-11)*


     
 
