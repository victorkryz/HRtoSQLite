/**
 * HRtoSQLite_tests
 *
 * @author Victor Kryzhanivskyi
 */


#include "SelectionTS.h"


class ReadingTablesTS : public SelectionTS {
};


TEST_F(ReadingTablesTS, ReadTable_LOCATIONS)
{
	const std::string srtStmt(Ora::SelectStatementGenerator(spConn_()).
										generateSelectStmt("LOCATIONS"));
	selectAndFetchToStdOut(srtStmt);
}

TEST_F(ReadingTablesTS, ReadTable_REGIONS)
{
	const std::string srtStmt(Ora::SelectStatementGenerator(spConn_()).
										generateSelectStmt("REGIONS"));
	selectAndFetchToStdOut(srtStmt);
}


TEST_F(ReadingTablesTS, ReadTable_COUNTRIES)
{
	const std::string srtStmt(Ora::SelectStatementGenerator(spConn_()).
										generateSelectStmt("COUNTRIES"));
	selectAndFetchToStdOut(srtStmt);
}

TEST_F(ReadingTablesTS, ReadTable_DEPARTMENTS)
{
	const std::string srtStmt(Ora::SelectStatementGenerator(spConn_()).
										generateSelectStmt("DEPARTMENTS"));
	selectAndFetchToStdOut(srtStmt);
}


TEST_F(ReadingTablesTS, ReadTable_EMPLOYEES)
{
	const std::string srtStmt(Ora::SelectStatementGenerator(spConn_()).
										generateSelectStmt("EMPLOYEES"));
	selectAndFetchToStdOut(srtStmt);
}

TEST_F(ReadingTablesTS, ReadTable_JOBS)
{
	const std::string srtStmt(Ora::SelectStatementGenerator(spConn_()).
										generateSelectStmt("JOBS"));
	selectAndFetchToStdOut(srtStmt);
}

TEST_F(ReadingTablesTS, ReadTable_JOB_HISTORY)
{
	const std::string srtStmt(Ora::SelectStatementGenerator(spConn_()).
										generateSelectStmt("JOB_HISTORY"));
	selectAndFetchToStdOut(srtStmt);
}








