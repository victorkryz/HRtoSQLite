/**
 * HRtoSQLite_tests
 *
 * @author Victor Kryzhanivskyi
 */

#include "../TestCommon.h"
#include "Ora/Utils.h"
#include "ConnectionHelper.h"


bool checkCurentSchemaForHR(const Ora::ConnectionSp& spConn);

TEST(TestIfHR_Test , TestIfHR)
{
    try
    {
        ConnectionHelper connHlp;
        connHlp.SetUp();

        ASSERT_TRUE(checkCurentSchemaForHR(connHlp.getConnection()));

        connHlp.TearDown();
    }
    catch (const std::exception& e)
    {
        std::string str(e.what());
        std::cerr << str << e.what() << std::endl;
        FAIL() << str.c_str();
    }
}

