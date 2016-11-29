/**
 * HRtoSQLite
 *
 * @author Victor Kryzhanivskyi
 */

#ifndef _ora_data_selector_h_
#define _ora_data_selector_h_

#include "Utils.h"
#include "Rowset.h"

#include "ocihwp.h"
#include "Describer.h"
#include "Poco/Format.h"

namespace Ora
{
	/**
	 * Selector class is used for performing selective SQL commands.
	 * It takes "SELECT .." statement, executes one and returns Rowset object.
	 */
	class Selector
	{
		protected:
			using fields_t = Rowset::fields_t;

		public:
			/**
			 * Class constructor
			 * @param conn 		connection object;
			 * @param strStmt 	"SELECT ..." statement;
			 */
			Selector(const Ora::ConnectionSp& conn, const std::string& strStmt);
			~Selector();

			Rowset select();
		
		private:
			fields_t buildFiledSet(Ora::StatementSp& stmt) const;
			FieldDefSp generateSelectionField(const Describer::field_info_t& fi) const;

			void defineFields(Ora::StatementSp& stmt, fields_t& fields) const;

		private:
			std::string strStmt_;
			Ora::ConnectionSp conn_;
	};


	/**
	 * This class generates "SELECT..." statement with explicit
	 * selection list for given table.
	 */
	class SelectStatementGenerator
	{
		public:
			SelectStatementGenerator(const Ora::ConnectionSp& conn) : conn_(conn){
			}

			std::string generateSelectStmt(const std::string& strTableName) const;
			std::string generateSelectionList(const std::string& strTableName) const;

		protected:
			const Ora::ConnectionSp conn_;
	};



}; // namespace Ora

#endif //_ora_data_selector_h_

