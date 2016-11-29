/**
 * HRtoSQLite_tests
 *
 * @author Victor Kryzhanivskyi
 */

#ifndef _tests_common_h_
#define _tests_common_h_


#include <stdio.h>
#include "gtest/gtest.h"
#include <string>


struct UnitTestParameters
{
	std::string strConnStr_;
	std::string strUserName_;
	std::string strUserPassword_;
	std::string strSqltDDLFile_;
	std::string strOutputSqltDbFile_;

	void initFromEnv();
	void show();
};

extern UnitTestParameters glTestParams;




#endif // _tests_common_h_
