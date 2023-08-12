/**
 * HRtoSQLite
 *
 * @author Victor Kryzhanivskyi
 */

#include "DbsWriter.h"
#include "StmtFileParser.h"
#include <Poco/Util/Application.h>
#include "Poco/Data/Session.h"
#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Exception.h"
#include "Poco/Data/SQLite/SQLiteException.h"
#include <Poco/File.h>
#include "../Common.h"


namespace Sqlt
{

	//& ******************************************************************************
	//& *  StructWriter implementation:
	//& ******************************************************************************

	void StructWriter::initStructure(bool bRecreate)
	{
		StmtFileParser parser(strStructFile_);
		parser.parse();

		if (bRecreate && doesExist(strResultFile_))
			Poco::File(strResultFile_).remove(false);

		SessionSp spSession(Utils::openSession(strResultFile_));
		scope_t<> scope([spSession](bool) noexcept {
			spSession->close();
		});

		Utils::executeCommands(spSession, parser.getStatements());
		scope.enclose();
	}

	bool StructWriter::doesExist(const std::string& strDbFile) {
		return Poco::File(strDbFile).exists();
	}



	//& *********************************************************************************
	//& *  Session implementation:
	//& *********************************************************************************

	Session::Session(const std::string& strDbFile) :
								strDbFile_(strDbFile)
	{
		session_ = Utils::openSession(strDbFile_);
		session_->begin();
	}

	Session::~Session()
	{
		if (nullptr != session_.get())
			close(true);
	}

	void Session::commit()
	{
		if (nullptr != session_.get())
			session_->commit();
	}

	void Session::rollback()
	{
		if (nullptr != session_.get())
			session_->rollback();
	}

	int Session::getRowCount(const std::string& strTableName)
	{
		using namespace Poco::Data::Keywords;

		const char* stmtTempl = "SELECT COUNT(*) FROM %s";
		std::string strResStmt(Poco::format(stmtTempl, strTableName));

		int count(0);
		*session_ << strResStmt, into(count), now;

		return count;
	}


	void Session::close(bool bCommit) noexcept
	{
		try
		{
			if (nullptr != session_.get())
			{
				if (session_->isConnected())
				{
					if (bCommit)
						session_->commit();

					session_->close();
				}

				session_.reset();
			}
		}
		catch (const std::exception& e)
		{
			const std::string strMsg(Poco::format("TableWriter::close() detected exception (%s))", e.what()));
			Poco::Util::Application::instance().logger().error(strMsg);
		}
	}



	//& *********************************************************************************
	//& *  TableWriter implementation:
	//& *********************************************************************************

	TableWriter::TableWriter(const std::string& strDbFile, const std::string& strTableName) :
								Session(strDbFile), strTableName_(strTableName) {
	}

	TableWriter::TableWriter(const std::string& strDbFile, const std::string& strTableName, 
							const Ora::Rowset::fields_t& fields) :
							Session(strDbFile), strTableName_(strTableName) {
		init(fields);
	}

	TableWriter::~TableWriter() {
			close(true);
	}

	void TableWriter::init(const Ora::Rowset::fields_t& fields)
	{
		strStmt_ = buildStmt(fields);

		session_ = Utils::openSession(strDbFile_);
		session_->begin();
	}

	std::string TableWriter::buildStmt(const Ora::Rowset::fields_t& fields) const
	{
		const char* stmtTempl = "INSERT INTO %s (%s) VALUES(%s)";
		std::string strNames, strVals;

		const auto fsz(fields.size());
		int pos(1);
		for (const auto f : fields)
		{
			strNames += f->getName();
			strVals += '?';
			if ( pos++ < fsz)
			{
				strNames += ", ";
				strVals += ", ";
			}
		}

		std::string strResStmt(Poco::format(stmtTempl, strTableName_, strNames, strVals));
		return strResStmt;
	}

	void TableWriter::insertRow(const Poco::Data::AbstractBindingVec& vals)
	{
		using Poco::Data::SQLite::InvalidSQLStatementException;

		Poco::Data::Statement stmt = *session_ << strStmt_;
		for (const auto& val : vals) {
			stmt.addBind(val);
		}

		try {
			stmt.execute();
		}
		catch (const InvalidSQLStatementException& e)
		{
			std::string strMsg(e.message());
			strMsg.append(" (" + strStmt_ + ")");
			throw InvalidSQLStatementException(strMsg, e.code());
		}
	}


	//& *********************************************************************************
	//& *  Utils implementation:
	//& *********************************************************************************

	SessionSp Utils::openSession(const std::string& strDbFile)
	{
		using Poco::Data::Session;
		using Poco::Data::SQLite::InvalidSQLStatementException;
		using namespace Poco::Data::Keywords;

		Poco::Data::SQLite::Connector::registerConnector();
		SessionSp spSession(std::make_shared<Session>("SQLite", strDbFile));;
		return spSession;
	}

	void Utils::executeCommands(SessionSp& spSession, const std::list<std::string>& commands)
	{
		using Poco::Data::SQLite::InvalidSQLStatementException;
		using namespace Poco::Data::Keywords;

		Poco::Data::Session& session = *spSession.get();

		for (auto stmt : commands)
		{
			try {
				session << stmt, now;
			}
			catch (const InvalidSQLStatementException& e)
			{
				std::string strMsg(e.message());
				strMsg.append(" (" + stmt + ")");
				throw InvalidSQLStatementException(strMsg, e.code());
			}
		}
	}




}; //namespace Sqlt
