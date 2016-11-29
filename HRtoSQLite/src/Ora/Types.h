/**
 * HRtoSQLite
 *
 * @author Victor Kryzhanivskyi
 */

#ifndef _customtypes_h_
#define _customtypes_h_

#include "ocihwp.h"
#include <memory>
#include <stdexcept>

namespace Ora
{
	using Env = ocihwp::OciHandle<OCIEnv>;
	using SvcCtx = ocihwp::OciHandle<OCISvcCtx>;
	using Error = ocihwp::OciHandle<OCIError>;
	using Server = ocihwp::OciHandle<OCIServer>;
	using Session = ocihwp::OciHandle<OCISession>;
	using Statement = ocihwp::OciHandle<OCIStmt>;
	using Define = ocihwp::OciHandle<OCIDefine>;
	using Describe = ocihwp::OciHandle<OCIDescribe>;

	using EnvSp = std::shared_ptr<Env>;
	using SvcCtxSp = std::shared_ptr<SvcCtx>;
	using ErrorSp = std::shared_ptr<Error>;
	using ServerSp = std::shared_ptr<Server>;
	using SessionSp = std::shared_ptr<Session>;
	using StatementSp = std::shared_ptr<Statement>;
	using DefineSp = std::shared_ptr<Define>;
	using DescribeSp = std::shared_ptr<Describe>;


	class Exception : public std::runtime_error
	{
	public:
		Exception(ub4 status) :
			status_(status), code_(-1), std::runtime_error("") {
		};

		Exception(ub4 status, const std::string& msg) :
			status_(status), msg_(msg), code_(-1), std::runtime_error(msg) {
		};

		Exception(ub4 status, sb4 code, const std::string& msg) :
			status_(status), code_(code), msg_(msg), std::runtime_error(msg) {
		};

		Exception(const Exception& e) :
			status_(e.status_), code_(e.code_), msg_(e.msg_), std::runtime_error(e.msg_) {
		};

	protected:
		sb4 code_;
		ub4 status_;
		std::string msg_;
	};

}; // namespace ora

#endif //_customtypes_h_
