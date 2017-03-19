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

    "select employee_id id, cast((rank_decorator ||' '|| full_name) as varchar2(40)) as full_name, job_title as job, "
            "cast(nvl2(department_id, department_name || ' (' || department_id ||')', '') as varchar2(30)) as department, "
            "nvl2(state_province, state_province || ' (' || country ||')','') as location , region_name as region, "
            "phone, email, salary, commission, job_id, location_id, country_id, region_id from "
        "(with sq as (select * from "
        "(select level l, employee_id,  first_name ||' ' || last_name as full_name, "
                "first_name, last_name, phone_number, email, salary, commission_pct, "
                "job_id, hire_date, department_id "
        "from employees e "
        "start with e.manager_id is null "
        "connect by prior e.employee_id = e.manager_id) xa "
            "left join (select distinct manager_id from employees) m on (xa.employee_id = m.manager_id)) "
                "select ea.employee_id as employee_id, first_name, last_name, ea.job_id as job_id, j.job_title as job_title, "
                "ea.phone_number as phone, ea.email as email, hire_date, ea.salary as salary, "
                "nvl(ea.commission_pct, 0) as commission, ea.manager_id as manager_id, nvl2(ea.manager_id, 1, 0) as is_manager, "
                "d.department_id as department_id, department_name, l.state_province as state_province, "
                "l.city as city, c.country_name as country, r.region_id as region_id, c.country_id, l.location_id, "
                "r.region_name as region_name, l as rank, full_name, lpad(' ',2+(l-1)) || '-' as rank_decorator "
                "from sq ea "
            "left join departments d on (ea.department_id = d.department_id) "
            "left join locations l on (d.location_id = l.location_id) "
            "left join countries c on (l.country_id = c.country_id) "
            "left join regions r on (c.region_id = r.region_id) "
            "left join locations l on (d.location_id = l.location_id) "
            "left join jobs j on (ea.job_id = j.job_id))";

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
