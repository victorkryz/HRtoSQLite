# HRtoSQLite
HRtoSQLite utility transfers Oracle Database sample HR schema's data into SQLite database.


#### Motivations:

- Get acquainted with the POCO C++. 
- Using POCO data access API for operating on SQLite database.
- Make it cross-platform (Windows/Linux OS).
 
 
#### Features:
 
 - Oracle call interface (OCI) is used to Oracle acces;
 - Poco features:
    - there's used POCO-proposed skeleton for application organizing (Application class); 
    - data access API (SQLite direction);
    - command-line parameters handling feature;
    - logging into application log;
    - some utilities things (e.g. string formatting)
 - C++ 11 features are used:
    - "auto" keyword, "move semantics", lambda functions, threading,  
       shared_ptr(s), enum, on-place class member initialization,   
       "using" specificator for custom-types definitions,   
       "override" for virtual functions, range-based for loop,   
       strongly typed enumerations, etc.
 - "Google's C++ test" as a unit testing framework.
 - CMake configuration files and tool;
 - IDE: CLion (for Linux), Visual Studio 2015 (for Windows);


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


- Download and install [*Poco C++ libraries*](https://pocoproject.org/download/)
- Download and install [*Oracle Instant Client (Package - SDK)*](http://www.oracle.com/technetwork/database/features/instant-client/index-097480.html)
- Download [*Google Test, Google's C++ test framework*](https://github.com/google/googletest) 

 *(Through the development there's used poco version 1.7.4, Oracle client 12.1, googletest-1.7.0)*

- #### For linux:

    The project building is based on CMake configuration files and built with cmake tool.  
    
    - go into file HRtoSQLite/CMakeLists.txt and set OCI and Poco paths in variables:  
      *OCI_INC_DIR, OCI_LIB_DIR, POCO_INC_DIR, POCO_LIB_DIR*;
    - go into file HRtoSQLite_tests/g.tests/CMakeLists.txt and set google test paths in variables:   
      *GTEST_INC_DIR, GTEST_LIB_DIR*

    Build the subprojects '*HRtoSQLite*' (static library), '*HRtoSQLite_run*' 

    For the development there's used CLion IDE: open project root directory to explore all projects' structure, build them and run.
   
    *To build it without IDE, using only cmake CLI, do steps like below:*  

        ```
        mkdir build-debug
        cd build-debug
        cmake -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - Unix Makefiles" ../.
        cmake --build ./ --target all
        ```

 - #### For Windows (Visual Studio 2015):

    -go into directory HRtoSQLite.VS2015 and open HRToSQLiteWS.sln solution;

        Projects sesttings suppose the next default pathes configuration.   

        ..\..\..\vendor\instantclient_12_1\sdk\include  - OCI headers
        ..\..\..\vendor\poco-1.7.4\include              - POCO headers
        ..\..\..\vendor\instantclient_12_1\sdk\lib\msvc - OCI libraries
        .\..\..\vendor\poco-1.7.4\lib64                 - POCO libraries

    *(note: there's only x64 configuration provided)*

    Simulate a similar directories disposition (e.g. using file system links) or substitute these
    directories on yours in projects settings (*"Additional Include Directories"*, *"Additional Library Directories"*)  


### Testing:
-------------------------------------------------------------------------

To get the unit tests runable, build the subproject *HRtoSQLite_tests*.  
There're provided a set of tests for Oracle connection, reading 'HR' schema tables, writing data into SQLite database, e.g.

The tests are parametrized by environment variables:

```
ORA_CONN_STR   Oracle connection string
ORA_NAME       Oracle user name
ORA_PSW        Oracle user password 
SQLT_DDL       HR-SQLite database structute intialization file
OUTPUT         SQLite database file (default: ./hr.db)
```

Directory 'Launchers' contains some configuration and scripts to help test launching.  
To launch unit tests from command line, setup variables in *'config.env'* file and launch *'testLauncher'* script.

**Warning:** in case these tests are launched under from IDE (CLion, VS 2015), setup given environment 
variables in the respective configuration (debug/run) before debuging or running some test(s)!  


-------------------------------------------------------------------------
Project built and tested under OS: *Windows 7 (64-bit)*, *Ubuntu 16.04 LTS (64-bit)*


     
 
