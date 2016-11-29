/**
 * HRtoSQLite
 *
 * @author Victor Kryzhanivskyi
 */


#include "Utils.h"
#include "ocihwp.h"
#include "Describer.h"

namespace Ora
{
    /**
     * Creates field info object
     *
     * @param param     OCI field descriptor parameter
     * @return          Describer::field_info_t object
     * */
    Describer::field_info_t Describer::buildFiledInfo(OCIParam* param) const
    {
        const auto& conn = *(conn_.get());
        const auto& spErr = conn.getError();

        constexpr ub4 targHndlType(OCI_DTYPE_PARAM);

        decltype(std::declval<field_info_t>().dt_) dt = 0;
        _OraCall(conn,
                 OCIAttrGet(param, targHndlType, &dt, nullptr,
                            OCI_ATTR_DATA_TYPE, spErr->get()));

        OraText* pName = nullptr;
        decltype(std::declval<field_info_t>().sz_) szName(0), sz(0);
        _OraCall(conn,
                 OCIAttrGet(param, targHndlType, &pName, &szName,
                            OCI_ATTR_NAME, spErr->get()));
        _OraCall(conn,
                 OCIAttrGet(param, targHndlType, &sz, nullptr,
                            OCI_ATTR_DATA_SIZE, spErr->get()));

        typename field_info_t::num_traits_t nt;

        if (dt == SQLT_NUM)
        {
            _OraCall(conn,
                     OCIAttrGet(param, targHndlType, &nt.precision_, nullptr,
                                OCI_ATTR_PRECISION, spErr->get()));

            _OraCall(conn,
                     OCIAttrGet(param, targHndlType, &nt.scale_, nullptr,
                                OCI_ATTR_SCALE, spErr->get()));
        }


        return{ std::string(reinterpret_cast<const char*>(pName), szName), dt, sz, nt };
    }

} // namespace Ora

