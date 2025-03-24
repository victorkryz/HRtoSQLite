/**
 * HRtoSQLite_tests
 *
 * @author Victor Kryzhanivskyi
 */

#include "TestCommon.h"
#include "Poco/AutoPtr.h"
#include "Poco/Environment.h"
#include "HRtoSQLiteApp.h"


#ifdef _WIN32
	UnitTestParameters glTestParams{ "//localhost:1521/xe", "hr", "hr" };
#else
	UnitTestParameters glTestParams{ "//192.168.xxx.xxx:1521/xe", "hr", "hr" };
#endif // _WIN32


GTEST_API_ int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);

    glTestParams.initFromEnv();
	glTestParams.show();

    char chPath[256] = {0,0};
    std::string strCurrDir(getcwd(chPath, sizeof(chPath)));
    std::cout << "Current directory: \"" << strCurrDir << "\"" << std::endl << std::endl;

	Poco::AutoPtr<HRtoSQLiteApp> pApp = new HRtoSQLiteApp;

	return RUN_ALL_TESTS();
}

void UnitTestParameters::initFromEnv()
{
	using namespace Poco;

	try {

		strConnStr_ = Environment::get("ORA_CONN_STR");
		strUserName_ = Environment::get("ORA_NAME");
		strUserPassword_ = Environment::get("ORA_PSW");
		strSqltDDLFile_ = Environment::get("SQLT_DDL");
		strOutputSqltDbFile_ = Environment::get("OUTPUT");
	}
	catch(const NotFoundException& e)
	{
		std::string srtMsg("Unit tests parameters initialization: ");
		std::cout << srtMsg << "environment variable not found (" << e.message() << ")" << std::endl;
        throw;
	}
}

void UnitTestParameters::show()
{
	std::ostream& stream = std::cout;

	stream << "******************************************************************************" << std::endl;
	stream << "Used parameters:" << std::endl;
	stream << "------------------------------------------------------------------------------" << std::endl;
	stream << " - Oracle connection string: " << strConnStr_ << std::endl;
	stream << " - Oracle user name: " << strUserName_ << std::endl;
	stream << " - Oracle login password: " << strUserPassword_ << std::endl;
	stream << " - SQLite-db DDL file path: " << strSqltDDLFile_ << std::endl;
	stream << " - SQLite-db result file path: " << strOutputSqltDbFile_ << std::endl;
	stream << "******************************************************************************" << std::endl;
}