/**
 * HRtoSQLite
 *
 * @author Victor Kryzhanivskyi
 */

#include "Interfaces.h"
#include "ExportProcess.h"
#include "Ora/Reader.h"
#include "Sqlt/DbsWriter.h"
#include <Poco/Util/Application.h>
#include "Ora/Selector.h"
#include "Common.h"


TableExporter::TableExporter(const Ora::ConnectionSp& spConn,
							 const std::string& strSqltDbFileName, IdleCallBackSp spIdleCallBack) :
								spConn_(spConn),
								strSqltDbFileName_(strSqltDbFileName),
								spIdleCallBack_(spIdleCallBack) {
}


void TableExporter::proceed(const std::string strTableName)
{
	const std::string srtStmt(Ora::SelectStatementGenerator(spConn_).
		generateSelectStmt(strTableName));

	Ora::Selector select(spConn_, srtStmt);
	auto rowset = select.select();

	try
	{
		Sqlt::TableWriter tblWriter(strSqltDbFileName_, strTableName, rowset.getFieldSet());
		scope_t<> scope([&tblWriter](bool bEnclosed) noexcept {
			tblWriter.close(bEnclosed);
		});

		Sqlt::RowBuilder rb(spConn_);
		while (rowset.fetchNext(rb))
		{
			const auto& rowVals = rb.getBindings();
			tblWriter.insertRow(rowVals);

			onRowInserted(rowVals);
		}

		scope.enclose();
	}
	catch (const std::exception&) 
	{
		const std::string strMsg(Poco::format("Export of table %s failed)", strTableName));
		std::throw_with_nested(std::runtime_error(strMsg));
	}
}

void TableExporter::onRowInserted(const Sqlt::bindings_t& rowVals)
{
	if (nullptr != spIdleCallBack_)
		spIdleCallBack_->onIdle();
}


