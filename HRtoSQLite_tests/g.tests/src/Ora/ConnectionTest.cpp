/**
 * HRtoSQLite_tests
 *
 * @author Victor Kryzhanivskyi
 */

#include "../TestCommon.h"
#include "Ora/Connection.h"
#include "ConnectionHelper.h"
#include <string>


TEST(OraConnectionTestCase, OraConnectionTest)
{
	try
	{
		ConnectionHelper connHlp;
		connHlp.SetUp();

		Ora::ConnectionSp& spConn = connHlp.getConnection();
		EXPECT_TRUE((spConn->getSvcCtx() != nullptr));

		connHlp.TearDown();
	} 
  catch (const std::exception& e)
	{
		std::string str(e.what());
		FAIL() << e.what();
	}
}