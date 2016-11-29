/**
 * HRtoSQLite
 *
 * @author Victor Kryzhanivskyi
 */

#include "Interfaces.h"
#include "ExportProcess.h"
#include "Ora/Connection.h"
#include "Sqlt/DbsWriter.h"
#include <Poco/Util/Application.h>
#include "BreakingHandler.h"
#include <thread>
#include <future>

ExportrProcess::ExportrProcess() : 
						reportStream_(nullptr)
{
	// fill table names list:
	tablesToExport_ = {
		"REGIONS",
		"COUNTRIES",
		"LOCATIONS",
		"DEPARTMENTS",
		"EMPLOYEES",
		"JOBS",
		"JOB_HISTORY"
	};

	// init dafault parameters:
	params_[Parameters::OraConnName] = "hr";
	params_[Parameters::SqltStructFile] = "././/sql/hrSQLiteDDL.sql";
	params_[Parameters::SqltResulFile] = "./hr.db";
}

void ExportrProcess::init(std::ostream* reportStream) {
	reportStream_ = reportStream;
}

bool checkCurentSchemaForHR(const Ora::ConnectionSp& spConn);

void ExportrProcess::startProcess()
{
	try
	{
		std::string connStr(obtainParam(Parameters::OraConnStr)),
					name(obtainParam(Parameters::OraConnName)),
					psw(obtainParam(Parameters::OraConnPsw));
		spConn_ = createConnection(connStr, name, psw);
		
		reinitSQLiteDbStruct();

        if ( !checkCurentSchemaForHR(spConn_) )
            throw std::runtime_error("It doesn't seems an \"HR\" schema!\nTry to connect with another user name!");

		Breaking::reactivateHandler();

		std::promise<bool> prom;
		auto fut = prom.get_future();

		std::thread wth([this](auto&& p) {
            this->launchTheCycle(std::move(p));
				}, std::move(prom));

		wth.join();

		Breaking::deactivateHandler();

		if (fut.get() )
			*reportStream_ << "The export process done." << std::endl;
	}
	catch (const cancel_occurrence&)
	{
		Breaking::setStatus(Breaking::BreakingStatus::Handled);

		std::string strMsg("The export process was cancelled.");
		Poco::Util::Application::instance().logger().error(strMsg);
	}
	catch (const std::exception& e)
	{
		std::string strMsg(e.what());
		Poco::Util::Application::instance().logger().error(strMsg);
	}
}

void ExportrProcess::reinitSQLiteDbStruct()
{
	const std::string structFile(obtainParam(Parameters::SqltStructFile)),
					  resulFile(obtainParam(Parameters::SqltResulFile));

	Sqlt::StructWriter(structFile, resulFile).
			              initStructure(true);
}


void ExportrProcess::launchTheCycle(std::promise<bool> &&prom)
{
	try
	{
		for (const auto& strName : tablesToExport_)
		{
			onBeforeTableExport(strName);
			exportTable(strName);
			onAfterTableExport(strName);
		}

		prom.set_value(true);
	}
	catch (const std::exception&)
	{
		try {
			prom.set_exception(std::current_exception());
		}
		catch (const std::future_error& fe) {
			Poco::Util::Application::instance().logger().error(fe.what());
		}
	}
}

void ExportrProcess::exportTable(const std::string strTableName)
{
	struct CallBack : public IdleCallback
	{
		CallBack(ExportrProcess& target) : target_(target){}
		void onIdle() override { target_.onIdle(); }
		ExportrProcess& target_;
	};

	TableExporter tbe(spConn_,
		obtainParam(Parameters::SqltResulFile),
					std::make_shared<CallBack>(*this));
		tbe.proceed(strTableName);
};


void ExportrProcess::onBeforeTableExport(const std::string& stTableName) {
	*reportStream_ << "Table \"" << stTableName << "\" ...";
}

void ExportrProcess::onAfterTableExport(const std::string& stTableName) 
{
	*reportStream_ << " done!" << std::endl;
	onIdle();
}

void ExportrProcess::onIdle()
{
	using namespace Breaking;

	if (BreakingStatus::Pending == Breaking::getStatus())
	{
		*reportStream_ << "Breaking ..." << std::endl;
		setStatus(BreakingStatus::Processing);
		throw cancel_occurrence(std::string("Cancelled by user"));
	}
}
