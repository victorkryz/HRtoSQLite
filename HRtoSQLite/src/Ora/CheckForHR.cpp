/**
 * HRtoSQLite
 *
 * @author Victor Kryzhanivskyi
 */

#include "Connection.h"
#include "Utils.h"

const std::string strStmt =
        "declare\n"
                "  v_rgns   REGIONS%rowtype;\n"
                "  v_cntr   COUNTRIES%rowtype;\n"
                "  v_locs   LOCATIONS%rowtype;\n"
                "  v_depts  DEPARTMENTS%rowtype;\n"
                "  v_empls  EMPLOYEES%rowtype;\n"
                "  v_jobs   JOBS%rowtype;\n"
                "  v_jobs_h JOB_HISTORY%rowtype;\n"
                "  \n"
                "begin\n"
                " null;\n"
                "end;";

/**
 * Checks if the specified schema confirms to "HR"
 *
 * @param spConn connection object
 * @return true if success
 */
bool checkCurentSchemaForHR(const Ora::ConnectionSp& spConn)
{
    bool bIsHRSchema(false);

    try {
            Ora::stmtExec(spConn, strStmt, false);
            bIsHRSchema = true;
    }
    catch (const Ora::Exception &) {
    }

    return bIsHRSchema;
}


