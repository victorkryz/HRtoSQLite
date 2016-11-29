/**
 * HRtoSQLite_tests
 *
 * @author Victor Kryzhanivskyi
 */

#include "Ora/Connection.h"
#include "Ora/Utils.h"
#include "Ora/Selector.h"
#include "Ora/Reader.h"
#include "TestCommon.h"
#include "Sqlt/DbsWriter.h"
#include "Ora/ConnectionHelper.h"


class IntegralTest : public ::testing::Test,
                            ConnectionHelper
{
	using CH = ConnectionHelper;

	class RowsCounter : public IdleCallback
	{
		public:
			RowsCounter(int& counter) :
				counter_(counter) {
			}

			virtual void onIdle() {
				counter_++;
			}

		private:
			int& counter_;
	};

	public:
		IntegralTest() {
		}

		virtual ~IntegralTest() {
		}
	protected:
		void exportTable(const std::string strTableName)
		{
			bool bSucceeded(true);
			int iteratedRowsCounter(0);

			try
			{
				//& Do a whole table export procedure
				//& with calculating of inserted rows:
				TableExporter tbe(spConn_(), strDbFile_, 
								  std::make_shared<RowsCounter>(iteratedRowsCounter));
				tbe.proceed(strTableName);

				//& Get rows number from the target SQLite table:
				Sqlt::Session session(strDbFile_);
				const int realRowCounter = session.getRowCount(strTableName);

				//& Check expectations:
				EXPECT_TRUE(iteratedRowsCounter==realRowCounter);

				std::cout << std::endl << "Table \"" << strTableName << "\" exported" << " (" << iteratedRowsCounter << " rows)." << std::endl;
			}
			catch (const std::exception& e)
			{
				bSucceeded = false;
				std::cerr << "Table " << strTableName << ", exception occured: " << e.what() << std::endl;
			}

			ASSERT_TRUE(bSucceeded);
		}

	protected:
		void SetUp() override 
		{
			CH::SetUp();

			Sqlt::StructWriter sqltWriter(strStructFile_, strDbFile_);
			sqltWriter.initStructure(true);
		}
		void TearDown() override {
			CH::TearDown();
		}

		Ora::ConnectionSp& spConn_() {
			return CH::spConn_;
		};

	protected:
		std::string strDbFile_ = glTestParams.strOutputSqltDbFile_;
		std::string strStructFile_ = glTestParams.strSqltDDLFile_;
};


TEST_F(IntegralTest, Export_ALL)
{
	exportTable("REGIONS");
	exportTable("COUNTRIES");
	exportTable("LOCATIONS");
	exportTable("DEPARTMENTS");
	exportTable("EMPLOYEES");
	exportTable("JOBS");
	exportTable("JOB_HISTORY");
}