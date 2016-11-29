/**
 * HRtoSQLite
 *
 * @author Victor Kryzhanivskyi
 */


#ifndef _connection_h_
#define _connection_h_

#include "oci.h"
#include "Types.h"

namespace Ora
{
	/**
	 * Connection is the class that represents oracle connection and
	 * encloses some OCI objects needed to keep connection context.
	 */
	class Connection
	{
	public:
		static Connection* createConnection(EnvSp& env, Ora::ServerSp& spSrv, 
											SvcCtxSp& svcCtx, SessionSp& session, ErrorSp& error) {
			return new Ora::Connection(env, spSrv, svcCtx, session, error);
		}

	public:
		Connection(EnvSp& env, ServerSp& spSrv, SvcCtxSp& svcCtx, SessionSp& session, ErrorSp& error) :
			env_(env), error_(error), spSrv_(spSrv), svcCtx_(svcCtx),  session_(session) {
		}
		~Connection()
		{
			session_ = nullptr;
			svcCtx_ = nullptr;
			spSrv_= nullptr;
			error_ = nullptr;
			env_ = nullptr;
		}

		const SvcCtxSp& getSvcCtx()	const {
			return svcCtx_;
		}
		const EnvSp& getEnv()	const {
			return env_;
		}
		const SessionSp& getSession()	const {
			return session_;
		}
		const ErrorSp& getError()	const {
			return error_;
		}


	protected:
		EnvSp env_;
		ErrorSp error_;
		ServerSp spSrv_;
		SvcCtxSp svcCtx_;
		SessionSp session_;
	};

	typedef std::shared_ptr<Connection> ConnectionSp;

};

Ora::ConnectionSp createConnection(const std::string& connStr, const std::string& name, const std::string& psw);

#endif //_connection_h_
