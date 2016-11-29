/**
 * HRtoSQLite
 *
 * @author Victor Kryzhanivskyi
 */
#ifndef _ora_describer_h_
#define _ora_describer_h_

#include "Utils.h"
#include "ocihwp.h"


namespace Ora
{
	/**
	 * Describer is the utility class intended to get field info
	 * from the OCI parameter represents a some filed of the prepared
	 * selective SQL statement
	 */
	class Describer
	{
	public:

		struct field_info_t
		{
			struct num_traits_t
			{
                num_traits_t() {}

				sb2 precision_ = 0;
				sb1 scale_ = 0;
			};

			field_info_t(std::string strName, ub2 dt, ub4 sz = 0, num_traits_t nt = num_traits_t()) :
				strName_(strName), dt_(dt), sz_(sz), nt_(nt) {
			}

			ub2 dt_ = 0;
			ub4 sz_ = 0;
			num_traits_t nt_;
			std::string strName_;
		};

	public:
		Describer(const Ora::ConnectionSp& spConn) : conn_(spConn) {}

		field_info_t buildFiledInfo(OCIParam* param) const;

    private:
		Ora::ConnectionSp conn_;
	};

}


#endif //_ora_describer_h_