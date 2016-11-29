/**
 * HRtoSQLite_tests
 *
 * @author Victor Kryzhanivskyi
 */

#include "SelectionTS.h"

class HRSelectionTS : public SelectionTS {
};


/**
 * A set of tests of "SELECT" statement
 * executed against to Oracle database ("HR" schema)
 */

//& Select "managers":
TEST_F(HRSelectionTS, Managers)
{
    std::cout << std::endl << "Select managers:" << std::endl;

    const std::string strStmt =
      "SELECT distinct * FROM "
              "(SELECT e.FIRST_NAME ||' ' || e.LAST_NAME as \"Manager\", d.DEPARTMENT_NAME as \"Department\" FROM  "
                    "EMPLOYEES e inner join EMPLOYEES m on (e.EMPLOYEE_ID = m.MANAGER_ID and m.MANAGER_ID is not null)  "
                                "inner join DEPARTMENTS d on (e.DEPARTMENT_ID = d.DEPARTMENT_ID)) "
                                                                                "order by \"Manager\"";
    selectAndFetchToStdOut(strStmt);
}

//& Select "managers" with extended info:
TEST_F(HRSelectionTS, ManagersWithExtraInfo)
{
    std::cout << std::endl << "Select managers with extra info:" << std::endl;

    const std::string strStmt =
    "SELECT distinct * FROM "
            "(SELECT e.FIRST_NAME ||' ' || e.LAST_NAME as \"Manager\", d.DEPARTMENT_NAME as \"Department\", "
                     "l.CITY as \"City\", l.STATE_PROVINCE as \"State\", c.COUNTRY_NAME as \"Country\", "
                     "r.REGION_NAME as \"Region\" "
                "FROM EMPLOYEES e inner join EMPLOYEES m on (e.EMPLOYEE_ID = m.MANAGER_ID and m.MANAGER_ID is not null) "
                                  "inner join DEPARTMENTS d on (e.DEPARTMENT_ID = d.DEPARTMENT_ID) "
                                  "inner join LOCATIONS l on (d.LOCATION_ID = l.LOCATION_ID) "
                                  "inner join COUNTRIES c on (l.COUNTRY_ID = c.COUNTRY_ID) "
                                  "inner join REGIONS r on (c.REGION_ID = r.REGION_ID)) order by \"Manager\"";
    selectAndFetchToStdOut(strStmt);
}

//& Select all the employee's hierarchy:
TEST_F(HRSelectionTS, SelectEmployeeHierarchy)
{
    std::cout << std::endl << "Select all the employee's hierarchy:" << std::endl;

    const std::string strStmt =
            "SELECT ea.EMPLOYEE_ID as \"Id\",  ea.\"Employee\", j.JOB_TITLE as \"Job\", ea.\"Email\", ea.\"Salary\",  ea.\"Commission(%)\", "
                    "NVL2(m.MANAGER_ID, 'Manager', '') as \"Position\", d.DEPARTMENT_NAME as \"Department\", l.STATE_PROVINCE as \"State\", "
                    "l.CITY as \"City\", c.COUNTRY_NAME as \"Country\", r.REGION_NAME as \"Region\" FROM "
                    "(SELECT LEVEL l, EMPLOYEE_ID, LPAD(' ',2+(LEVEL-1)) || '- ' || FIRST_NAME ||' ' || LAST_NAME as \"Employee\", "
                    "EMAIL as \"Email\", SALARY as \"Salary\", COMMISSION_PCT as \"Commission(%)\", JOB_ID "
                "FROM EMPLOYEES e START WITH e.MANAGER_ID is NULL CONNECT BY PRIOR e.EMPLOYEE_ID = e.MANAGER_ID) ea "
                        "left join (SELECT distinct MANAGER_ID FROM EMPLOYEES) m on (ea.EMPLOYEE_ID = m.MANAGER_ID) "
                        "left join DEPARTMENTS d on (ea.EMPLOYEE_ID = d.MANAGER_ID) "
                        "left join LOCATIONS l on (d.LOCATION_ID = l.LOCATION_ID) "
                        "left join COUNTRIES c on (l.COUNTRY_ID = c.COUNTRY_ID) "
                        "left join REGIONS r on (c.REGION_ID = r.REGION_ID) "
                        "left join LOCATIONS l on (d.LOCATION_ID = l.LOCATION_ID) "
                        "left join JOBS j on (ea.JOB_ID = j.JOB_ID)";
    selectAndFetchToStdOut(strStmt);
}

//& Select min/avg/max sallary of each kind of job in each department:
TEST_F(HRSelectionTS, SelectSalaryStatistic)
{
    std::cout << std::endl << "Select sallary statistic (by departments/jobs):" << std::endl;

    const std::string strStmt =
    "SELECT mms.DEPARTMENT_NAME ||  NVL2(DEPARTMENT_ID, '(' || DEPARTMENT_ID || ')', '') as \"Department(id)\", j.JOB_TITLE as \"Job\", "
            "\"People(number)\", \"Salary(max)\", \"Salary(avg)\", \"Salary(min)\" "
            "FROM ( SELECT count(EMPLOYEE_ID) as \"People(number)\", JOB_ID, min(SALARY) as \"Salary(min)\", "
                            "max(SALARY) as \"Salary(max)\", ROUND(avg(SALARY)) as \"Salary(avg)\", e.DEPARTMENT_ID, d.DEPARTMENT_NAME "
                    "FROM EMPLOYEES e left join DEPARTMENTS d on (e.DEPARTMENT_ID = d.DEPARTMENT_ID) "
                            "group by(e.DEPARTMENT_ID, d.DEPARTMENT_NAME, e.JOB_ID) order by e.DEPARTMENT_ID, \"Salary(min)\" desc) mms "
            "left join JOBS j on (mms.JOB_ID = j.JOB_ID) order by mms.\"Salary(avg)\" desc";

    selectAndFetchToStdOut(strStmt);
}
