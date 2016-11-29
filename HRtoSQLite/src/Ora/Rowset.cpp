/**
 * HRtoSQLite
 *
 * @author Victor Kryzhanivskyi
 */

#include "Utils.h"
#include "ocihwp.h"
#include "Rowset.h"

namespace Ora
{
	Rowset::Rowset(Ora::ConnectionSp& conn, Ora::StatementSp&& stmt, fields_t&& fields) : conn_(conn),
		stmt_(std::move(stmt)), fields_(std::move(fields)) {
	}

	Rowset::Rowset(Rowset&& other)
	{
		stmt_ = std::move(other.stmt_);
		conn_ = std::move(other.conn_);
		fields_ = std::move(other.fields_);
	}

	Rowset::~Rowset()
	{
		stmt_.reset();
		fields_.clear();
		conn_.reset();
	}

	bool Rowset::fetchNext(RowValuesListener& listener)
	{
		auto bResult = fetchNext();
		if (bResult)
			processRowValues(listener);

		return bResult;
	}

	bool Rowset::fetchNext()
	{
		const Ora::Connection& conn = *(conn_.get());
		const Ora::ErrorSp& spErr = conn.getError();

		auto status =
			_OraCall(conn,
				OCIStmtFetch2(stmt_->get(), spErr->get(), 1, OCI_FETCH_NEXT, 0, OCI_DEFAULT));

		return status == OCI_SUCCESS;
	}

	void Rowset::processRowValues(RowValuesListener& listener) const
	{
		listener.onIterationBegin(fields_);

		int fOrder(1);
		for (auto field : fields_)
		{
			switch (field->getDty())
			{
				case SQLT_VNU:
				{
					auto p = static_cast<RowValuesListener::FDNumber*>(field.get());
					listener.onRowValue(fOrder, *p);
					break;
				}
				case SQLT_VST:
				{
					auto p = static_cast<RowValuesListener::FDString*>(field.get());
					listener.onRowValue(fOrder, *p);
					break;
				}
				case SQLT_DAT:
				{
					auto p = static_cast<RowValuesListener::FDDate*>(field.get());
					listener.onRowValue(fOrder, *p);
					break;
				}
				case SQLT_TIMESTAMP:
				{
					auto p = static_cast<RowValuesListener::FDDateTime*>(field.get());
					listener.onRowValue(fOrder, *p);
					break;
				}
				default:
				{
					listener.onRowValue(fOrder, *(field.get()));
					break;
				}
			}

			fOrder++;
		}

		listener.onIterationEnd();
	}
}; // namespace Ora