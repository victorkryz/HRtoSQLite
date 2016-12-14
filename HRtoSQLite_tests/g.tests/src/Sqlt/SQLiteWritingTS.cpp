/**
 * HRtoSQLite_tests
 *
 * @author Victor Kryzhanivskyi
 */

#include "Ora/Connection.h"
#include "Ora/Utils.h"
#include "Ora/Selector.h"
#include "Ora/Reader.h"
#include "../TestCommon.h"
#include "Sqlt/DbsWriter.h"
#include "Sqlt/Transformation.h"


class SQLiteWritingTS : public ::testing::Test
{
	public:
		SQLiteWritingTS() {
		}

		~SQLiteWritingTS() {
		}

	protected:
		virtual void SetUp()
		{
			initConnection();

			Sqlt::StructWriter sw(strStructFile_, strDbFile_);
			sw.initStructure(true);
		}

		virtual void TearDown() {
		}

		
		void initConnection()
		{
			Ora::ConnectionSp spConn(createConnection(glTestParams.strConnStr_,
													  glTestParams.strUserName_,
													  glTestParams.strUserPassword_));
			spConn_ = spConn;
			spRowReader_ = std::unique_ptr<Ora::RowReaderToStream>(new Ora::RowReaderToStream(spConn_, std::cout));
		}

		void deinitConnection() {
			spConn_.reset();
		}

		void exportTable(const std::string strTableName)
		{
			bool bSucceeded(true);

			try
			{
				const std::string srtStmt(Ora::SelectStatementGenerator(spConn_).
					generateSelectStmt(strTableName));

				Ora::Selector select(spConn_, srtStmt);
				auto rowset = select.select();

				Sqlt::TableWriter tblWriter(strDbFile_, strTableName, rowset.getFieldSet());

				Sqlt::RowBuilder rb(spConn_);
				while (rowset.fetchNext(rb))
				{
					const auto& vals = rb.getBindings();
					tblWriter.insertRow(vals);
				}

				std::cout << std::endl << "Done!" << std::endl;
			}
			catch (const std::exception& e)
			{
				bSucceeded = false;
				std::cerr << "Table " << strTableName <<", exception occured: " << e.what() << std::endl;
			}

			ASSERT_TRUE(bSucceeded);
		}


	protected:
		Ora::ConnectionSp spConn_;
		std::unique_ptr<Ora::RowReaderToStream> spRowReader_;
		std::string strDbFile_ = glTestParams.strOutputSqltDbFile_;
		std::string strStructFile_ = glTestParams.strSqltDDLFile_;
};

TEST_F(SQLiteWritingTS, Export_LOCATIONS)
{
	exportTable("LOCATIONS");
}

TEST_F(SQLiteWritingTS, Export_REGIONS)
{
	exportTable("REGIONS");
}

TEST_F(SQLiteWritingTS, Export_COUNTRIES)
{
	exportTable("COUNTRIES");
}

TEST_F(SQLiteWritingTS, Export_DEPARTMENTS)
{
	exportTable("DEPARTMENTS");
}

TEST_F(SQLiteWritingTS, Export_EMPLOYEES)
{
	exportTable("EMPLOYEES");
}

TEST_F(SQLiteWritingTS, Export_JOBS)
{
	exportTable("JOBS");
}

TEST_F(SQLiteWritingTS, Export_JOB_HISTORY)
{
	exportTable("JOB_HISTORY");
}



