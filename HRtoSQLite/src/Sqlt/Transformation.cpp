/**
 * HRtoSQLite
 *
 * @author Victor Kryzhanivskyi
 */

#include "Transformation.h"


namespace Sqlt
{

	RowBuilder::RowBuilder(const Ora::ConnectionSp& spConn)
		: conv_(spConn) {
	}

	void RowBuilder::onIterationBegin(const FDSet& fields) {
		bindings_.resize(fields.size());
	}

	void RowBuilder::onIterationEnd() {
	}

	void RowBuilder::onRowValue(int fieldIndex, FDNumber& val)
	{
		using ulong_t = unsigned long;

		if (val.isNULL())
			addValue(fieldIndex, ulong_t(), true);
		else
		{
			if (conv_.OCINumberIsInt(val.value_))
			{
				unsigned long num(-1);
				conv_.OCINumberToInt(val.value_, num, false);
				addValue(fieldIndex, num);
			}
			else
			{
				float num(-1);
				conv_.OCINumberToReal(val.value_, num, false);
				addValue(fieldIndex, num);
			}
		}
	}

	void RowBuilder::onRowValue(int fieldIndex, FDString& val)
	{
		if (val.isNULL())
			addValue(fieldIndex, std::string(), true);
		else
		{
			std::string str(conv_.OCIStrToStdStr(val.getVal()));
			addValue(fieldIndex, str);
		}
	}

	void RowBuilder::onRowValue(int fieldIndex, FDDate& val)
	{
		if (val.isNULL())
			addValue(fieldIndex, std::string(), true);
		else
		{
			std::string str(conv_.OCIDateToStdStr(val.getVal()));
			addValue(fieldIndex, str);
		}
	}

	void RowBuilder::onRowValue(int fieldIndex, FDDateTime& val)
	{
		if (val.isNULL())
			addValue(fieldIndex, std::string(), true);
		else
		{
			std::string str(conv_.OCIDateTimeToStdStr(val.getVal()));
			addValue(fieldIndex, str);
		}
	}

	void RowBuilder::onRowValue(int fieldIndex, FieldDef& val) {
		addValue(fieldIndex, "[unsupported type]");
	}

}; //namespace Sqlt