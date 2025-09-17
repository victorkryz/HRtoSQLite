/**
 * HRtoSQLite
 *
 * @author Victor Kryzhanivskyi
 */

#ifndef _transformation_h_
#define _transformation_h_

#include "Poco/Data/Row.h"
#include "../Interfaces.h"
#include "../Ora/Rowset.h"
#include <Poco/Data/Session.h>
#include "Poco/Data/AbstractBinding.h"
#include "Poco/Data/Statement.h"
#include "Poco/Nullable.h"
#include "Poco/Version.h"

namespace Sqlt
{
	using bindings_t = Poco::Data::AbstractBindingVec;

	/**
	 * RowBuilder class implements RowValuesListener interface.
	 * It transforms values taken from OCI interface into a set
	 * of "AbstractBinding" objects that represents a row of values
	 * already prepared to bind them to SQL statement as an input data.
	 */
	class RowBuilder : public RowValuesListener
	{
		public:
			RowBuilder(const Ora::ConnectionSp& spConn);

			void onIterationBegin(const FDSet& fields) override;
			void onIterationEnd() override;
			void onRowValue(int fieldIndex, FDNumber& val) override;
			void onRowValue(int fieldIndex, FDString& val) override;
			void onRowValue(int fieldIndex, FDDate& val) override;
			void onRowValue(int fieldIndex, FDDateTime& val) override;
			void onRowValue(int fieldIndex, FieldDef& val) override;

			const bindings_t& getBindings() const {
				return bindings_;
			}

		private:
			template <class T> void addValue(int fieldIndex, const T& val, bool bIsNULL = false)
			{
				using namespace Poco::Data::Keywords;

				Poco::Data::AbstractBinding::Ptr spBinding = bIsNULL ?  bind(null_) : bind(val);
				bindings_[static_cast<size_t>(fieldIndex - 1)] = spBinding;
			}

           	#if POCO_VERSION < 0x010E0200
				Poco::NullType null_ = Poco::NullType::NULL_GENERIC;
			#else	
				Poco::NullType null_ = std::nullopt;
			#endif
			DTConv conv_;
			bindings_t bindings_;
	};
	
} // namespace Sqlt


#endif  // _transformation_h_

