/**
 * HRtoSQLite
 *
 * @author Victor Kryzhanivskyi
 */

#include "../Common.h"
#include "Connection.h"
#include "Utils.h"

Ora::Connection* createConnection(Ora::EnvSp& spEnv, Ora::SvcCtxSp& spSvc, Ora::ErrorSp& spErr,
									const std::string& connStr, const std::string& name, const std::string& psw);

/**
 * Creates a connection object
 *
 * @param connStr 	a connection string
 * @param name 		an user name
 * @param psw 		a password
 *
 * @return			the connection object
 * @throws 			Ora::Exception in case incorrect parameters or Oracle server isn't accessible
 */
Ora::ConnectionSp createConnection(const std::string& connStr, const std::string& name, const std::string& psw)
{
	sword swStatus(OCI_SUCCESS);

	ub4 mode(OCI_OBJECT | OCI_THREADED | OCI_NO_UCB);

	OCIEnv* pEnv = nullptr;
	swStatus = OCIEnvCreate(&pEnv, mode, nullptr,
							nullptr, nullptr, nullptr,
							0, nullptr);
	Ora::EnvSp spEnv;
	if (swStatus != OCI_SUCCESS)
		throw Ora::Exception(swStatus, 0, std::string("Cannot create OCI environment"));
	else
	{
		auto sp(std::make_shared<Ora::Env>(pEnv));
		spEnv.swap(sp);
	}

	auto err = ocihwp::HandleFactory<OCIError>::allocate(spEnv->get());
	auto spErr(std::make_shared<Ora::Error>(err));

	auto ctx = ocihwp::HandleFactory<OCISvcCtx>::allocate(spEnv->get());
	auto spSvcCtx(std::make_shared<Ora::SvcCtx>(ctx));

	Ora::ConnectionSp spResConn;

	try
	{
		Ora::ConnectionSp spConn(createConnection(spEnv, spSvcCtx, spErr,
													connStr, name,  psw));
		spResConn.swap(spConn);
	}
	catch (const Ora::Exception& e) {
		throw e;
	}

	return spResConn;
}



Ora::Connection* createConnection(Ora::EnvSp& spEnv, Ora::SvcCtxSp& spSvc, Ora::ErrorSp& spErr,
								  const std::string& connStr, const std::string& name, const std::string& psw)
{
	using namespace Ora;

	auto srv = ocihwp::HandleFactory<OCIServer>::allocate(spEnv->get());
	auto spSrv(std::make_shared<Ora::Server>(srv));

	auto status = OCIServerAttach(spSrv->get(), spErr->get(),
									cast_const_oratext(connStr.c_str()),
									static_cast<sb4>(connStr.size()), OCI_DEFAULT);
	if (status != OCI_SUCCESS)
	{
		sb4  sbCode(0);
		std::string msg;
		Ora::extractErrorInfo(spErr->get(), status, sbCode, msg);
		throw Ora::Exception(status, sbCode, msg);
	}

	status = OCIAttrSet(spSvc->get(), OCI_HTYPE_SVCCTX, spSrv->get(), static_cast<ub4>(0), OCI_ATTR_SERVER, spErr->get());
	if (status != OCI_SUCCESS)
		throw Ora::Exception(status);

	OCISession* ses = ocihwp::HandleFactory<OCISession>::allocate(spEnv->get());
	Ora::SessionSp spSes(std::make_shared<Ora::Session>(ses));

	status = OCIAttrSet(spSes->get(), OCI_HTYPE_SESSION, cast_oratext(name.c_str()),
						static_cast<sb4>(name.size()), OCI_ATTR_USERNAME, spErr->get());
	if (status != OCI_SUCCESS)
		throw Ora::Exception(status);

	status = OCIAttrSet(spSes->get(), OCI_HTYPE_SESSION, cast_vptr(psw.c_str()),
		static_cast<sb4>(psw.size()), OCI_ATTR_PASSWORD, spErr->get());
	if (status != OCI_SUCCESS)
		throw Ora::Exception(status);

	status = OCIAttrSet(spSvc->get(), OCI_HTYPE_SVCCTX, spSes->get(), static_cast<ub4>(0), OCI_ATTR_SESSION, spErr->get());
	if (status != OCI_SUCCESS)
		throw Ora::Exception(status);

	status = OCISessionBegin(spSvc->get(), spErr->get(), spSes->get(), OCI_CRED_RDBMS, OCI_DEFAULT);
	if (status != OCI_SUCCESS)
		throw Ora::Exception(status);

	return Ora::Connection::createConnection(spEnv, spSrv, spSvc, spSes, spErr); 
}

