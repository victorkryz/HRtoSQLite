#!/bin/bash

# This is a program that keeps your address book up to date.
exit_item="exit test launcher"
CNc='\033[0m'
CRed='\033[0;31m'
CGreen='\033[0;32m'
CTestName=$CGreen

tests=( "$exit_item"
      OraConnectionTestCase.OraConnectionTest
	    ReadingTablesTS.ReadTable_REGIONS
	    ReadingTablesTS.ReadTable_LOCATIONS
	    ReadingTablesTS.ReadTable_COUNTRIES
	    ReadingTablesTS.ReadTable_DEPARTMENTS
	    ReadingTablesTS.ReadTable_EMPLOYEES
	    ReadingTablesTS.ReadTable_JOBS
	    ReadingTablesTS.ReadTable_JOB_HISTORY
	    ReadingTablesTS.*
	    HRSelectionTS.Managers
	    HRSelectionTS.ManagersWithExtraInfo
	    HRSelectionTS.SelectEmployeeHierarchy
	    HRSelectionTS.SelectSalaryStatistic
	    HRSelectionTS.*
	    SQLiteWritingTS.Export_REGIONS
	    SQLiteWritingTS.Export_LOCATIONS
	    SQLiteWritingTS.Export_COUNTRIES
	    SQLiteWritingTS.Export_DEPARTMENTS
	    SQLiteWritingTS.Export_EMPLOYEES
	    SQLiteWritingTS.Export_JOBS
	    SQLiteWritingTS.Export_JOB_HISTORY
	    SQLiteWritingTS.*
	    IntegralTest.Export_ALL
	)


continue=1

while [ "$continue" == 1 ];
do
        sz=${#tests[@]}

        for (( i=0; i<sz; i++ ));
        do
           # echo "    " $((${i}+1)). ${CGREEN} ${tests[${i}]}${CNC}
            echo -e "    " $((${i}+1)). ${CTestName} ${tests[${i}]}${CNc}
        done

        echo "Select test (action) you'd like to launch (print an order and press enter):"
        read test_order

        selected_test=${tests[$((${test_order}-1))]}
        echo Selected action: ${selected_test}:

        if [ "$selected_test" = "$exit_item" ];
         then
            continue=0
        else
           ./startSingleTest.sh ${selected_test}
           echo -e "Press eny key to continue..."
           read -n 1
        fi
done

