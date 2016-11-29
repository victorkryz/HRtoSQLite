/**
 * HRtoSQLite
 *
 * @author Victor Kryzhanivskyi
 */

#ifndef _ora_reader_h_
#define _ora_reader_h_

#include "Connection.h"
#include "Utils.h"
#include "../Interfaces.h"
#include <string>

namespace Ora
{
    /**
     * RowReaderToStream implements RowValuesListener interface.
     * It is intended for writing row values into std::ostream object.
     */
	class RowReaderToStream : public RowValuesListener
	{
	public:
		RowReaderToStream(const Ora::ConnectionSp& spConn, std::ostream& stream);

		void onIterationBegin(const FDSet&) override;
		void onIterationEnd() override;
		void onRowValue(int, FDNumber& val) override;
		void onRowValue(int, FDString& val) override;
		void onRowValue(int, FDDate& val) override;
		void onRowValue(int, FDDateTime& val) override;
		void onRowValue(int fOrder, FieldDef& val) override;

	protected:
		bool treateAsNULL(FieldDef& val);
		template <class T> void output(const T& val) {
			stream_ << val << ", ";
		}

	protected:
		DTConv conv_;
		std::ostream& stream_;
		const std::string _strFormat = "DD-MON-YY";
		const std::string _strLang = "American";
	};
}; // namespace Ora



#endif //_ora_reader_h_