/**
 * HRtoSQLite
 *
 * @author Victor Kryzhanivskyi
 */

#ifndef _ora_rowset_h_
#define _ora_rowset_h_

#include "Utils.h"

#include "../Interfaces.h"
#include "Poco/Format.h"	

namespace Ora
{
	/**
	 * Rowset class is intended for fetching data from given
	 * selection, represented by Statement object.
	 */
	class Rowset
	{
		public:
			using fields_t = std::vector<FieldDefSp>;

			/**
			 * Class constructor:
			 *
			 * @param conn		connection object;
			 * @param stmt		executed statement object;
			 * @param fields 	collection of meta-info objects about selected fields;
			 */
			Rowset(Ora::ConnectionSp& conn, Ora::StatementSp&& stmt, fields_t&& fields);
			/**
			 * Moving constructor:
			 */
			Rowset(Rowset&& other);

			Rowset(const Rowset& other) = delete;
			Rowset& operator=(const Rowset& other) = delete;

			~Rowset();

			const fields_t& getFieldSet() const {
				return fields_;
			}

			bool fetchNext(RowValuesListener& listener);
			bool fetchNext();

		protected:
			void processRowValues(RowValuesListener& listener) const;

			Ora::StatementSp stmt_;
			fields_t fields_;
			Ora::ConnectionSp conn_;
		};

}; // namespace Ora

#endif //_ora_rowset_h_