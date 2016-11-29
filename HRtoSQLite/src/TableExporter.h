/**
 * HRtoSQLite
 *
 * @author Victor Kryzhanivskyi
 */

#ifndef _table_exporter_h_
#define _table_exporter_h_

#include "Interfaces.h"
#include "Sqlt/Transformation.h"

class TableExporter
{
	public: 
		using IdleCallBackSp = std::shared_ptr<IdleCallback>;

		TableExporter(const Ora::ConnectionSp& spConn,
					 const std::string& strSqltDbFileName, IdleCallBackSp spIdleCallBack = IdleCallBackSp());
		virtual ~TableExporter() {};

		void proceed(const std::string strTableName);

	protected:
		virtual void onRowInserted(const Sqlt::bindings_t& rowVals);
	
	protected:
		const Ora::ConnectionSp spConn_;
		std::string strSqltDbFileName_;
		IdleCallBackSp spIdleCallBack_;
};

#endif //_table_exporter_h_