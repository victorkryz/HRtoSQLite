/**
 * HRtoSQLite_tests
 *
 * @author Victor Kryzhanivskyi
 */

#ifndef _connection_helper_h_
#define _connection_helper_h_

#include "Ora/Connection.h"
#include <string>
#include <iostream>


class ConnectionHelper 
{
	public:
		ConnectionHelper() {
		}

		virtual ~ConnectionHelper() {
		}

		virtual void SetUp()
		{
			initConnection();
		}
		virtual void TearDown()
		{
			deinitConnection();
		}

		Ora::ConnectionSp& getConnection() {
			return spConn_;
		}

	protected:
		void initConnection()
		{
			Ora::ConnectionSp spConn(createConnection(glTestParams.strConnStr_,
													  glTestParams.strUserName_,
													  glTestParams.strUserPassword_));
			spConn_ = spConn;
		}

		void deinitConnection() {
			spConn_.reset();
		}

	protected:
		Ora::ConnectionSp spConn_;
};






#endif  // _connection_helper_h_
