/**
 * HRtoSQLite
 *
 * @author Victor Kryzhanivskyi
 */

#ifndef _sqlite_writer_h
#define _sqlite_writer_h

#include <string>
#include <list>
#include "../Interfaces.h"
#include "../Ora/Utils.h"
#include "../Ora/Rowset.h"
#include <Poco/Data/Row.h>
#include <Poco/Data/Session.h>
#include "../TableExporter.h"


namespace Sqlt
{
	using SessionSp = std::shared_ptr<Poco::Data::Session>;

	/**
	 * StructWriter class is intended to generate an initial SQLite database structure.
	 * It takes a file with DDL commands delimited by ";" and executes them consecutively.
	 */
	class StructWriter
	{
		public:
			static bool doesExist(const std::string& strDbFile);

			StructWriter(const std::string& strStructFile, const std::string& strResultFile) :
						strStructFile_(strStructFile), strResultFile_(strResultFile) {
			}

			void initStructure(bool bRecreate);

		protected:
			std::string strStructFile_,
						strResultFile_;

	};

	class Session
	{
		public:
			Session(const std::string& strDbFile);
		   ~Session();

			void commit();
			void rollback();
			int getRowCount(const std::string& strTableName);

			/**
			 * Explicit closing of the session.
			 * @param bCommit 	forces commiting before closing;
			 */
			void close(bool bCommit) noexcept;

		protected:
			std::string strDbFile_;
			SessionSp session_;

	};

	/**
	 * TableWriter class provides API for writing data into specified table.
	 */

	class TableWriter : public Session
	{
		public:

			/**
			 * Class constructors:
		     *	-explicitly opens a database on the construction phase
			 * 	 and closes on destruction with transaction committing.
			 *
			 * @param strDbFile 	a SQLite database file path;
			 * @param strTableName	a target table name;
			 * @param fields		field (column) set that reflects of the row structure
			 * 						and serves for generating "INSERT..." statement;
			 */
			TableWriter(const std::string& strDbFile, const std::string& strTableName);
			TableWriter(const std::string& strDbFile, const std::string& strTableName, const Ora::Rowset::fields_t& fields);
			~TableWriter();

			void init(const Ora::Rowset::fields_t& fields);
			/**
			 * Inserts a row into table with given values.
			 * @param vals	values for the row;
			 */
			void insertRow(const Poco::Data::AbstractBindingVec& vals);

		protected:
			std::string buildStmt(const Ora::Rowset::fields_t& fields) const;

		protected:
			std::string strTableName_,
						strStmt_;
	};

	namespace Utils
	{
		static SessionSp openSession(const std::string& strDbFile);
		static void executeCommands(SessionSp& spSession, const std::list<std::string>& commands);
	};



}; // Sqlt


#endif //_sqlite_writer_h

