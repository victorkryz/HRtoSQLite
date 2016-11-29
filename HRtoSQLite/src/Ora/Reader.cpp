/**
 * HRtoSQLite
 *
 * @author Victor Kryzhanivskyi
 */

#include "Reader.h"
#include <ostream>

namespace Ora
{
	RowReaderToStream::RowReaderToStream(const Ora::ConnectionSp& spConn, std::ostream& stream)
		: conv_(spConn), stream_(stream) {
	}


	void RowReaderToStream::onIterationBegin(const FDSet&) {
		stream_ << std::endl;
	}

	void RowReaderToStream::onIterationEnd() {
	}

	void RowReaderToStream::onRowValue(int, FDNumber& val)
	{
		if (!treateAsNULL(val))
		{
			if (conv_.OCINumberIsInt(val.value_))
			{
				unsigned long num(-1);
				conv_.OCINumberToInt(val.value_, num, false);
				output(num);
			}
			else
			{
				float num(-1);
				conv_.OCINumberToReal(val.value_, num, false);
				output(num);
			}
		}
	}

	void RowReaderToStream::onRowValue(int, FDString& val)
	{
		if (!treateAsNULL(val))
		{
			std::string str(conv_.OCIStrToStdStr(val.getVal()));
			output(str);
		}
	}

	void RowReaderToStream::onRowValue(int, FDDate& val)
	{
		if (!treateAsNULL(val))
		{
			std::string str(conv_.OCIDateToStdStr(val.getVal(), _strFormat, _strLang));
			output(str);
		}
	}

	void RowReaderToStream::onRowValue(int, FDDateTime& val)
	{
		if (!treateAsNULL(val))
		{
			std::string str(conv_.OCIDateTimeToStdStr(val.getVal(), _strFormat, _strLang));
			output(str);
		}
	}

	void RowReaderToStream::onRowValue(int fOrder, FieldDef& val) {
		output("[unsupported type]");
	}


	bool RowReaderToStream::treateAsNULL(FieldDef& val)
	{
		bool bResult(val.isNULL());
		if (bResult)
			stream_ << "NULL" << ",";
		return bResult;
	}

}; // namespace Ora