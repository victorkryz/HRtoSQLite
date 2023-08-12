/**
 * HRtoSQLite
 *
 * @author Victor Kryzhanivskyi
 */

#include "../Common.h"
#include "Utils.h"
#include "ocihwp.h"
#include "Selector.h"

namespace Ora {
	Selector::Selector(const Ora::ConnectionSp &conn, const std::string &strStmt) : conn_(conn),
																					strStmt_(strStmt) {
	}

	Selector::~Selector() {
	}

	Rowset Selector::select()
	{
		StatementSp stmt(stmtExec(conn_, strStmt_));

		auto fields = buildFiledSet(stmt);
		defineFields(stmt, fields);

		Rowset rws(conn_, std::move(stmt), std::move(fields));
		return rws;
	}

	Rowset::fields_t Selector::buildFiledSet(Ora::StatementSp &stmt) const {
		const auto &conn = *(conn_.get());
		const auto &spErr = conn_.get()->getError();

		ub4 order(0);
		sword status(OCI_ERROR);

		fields_t fields;
		do {
			OCIParam *paramDescr = nullptr;
			status = OCIParamGet(stmt->get(), OCI_HTYPE_STMT,
								 spErr->get(), (void **) &paramDescr, ++order);
			if (status == OCI_SUCCESS) {
				Describer descr(conn_);

				scope_t<> scope([paramDescr](bool) noexcept {
					OCIDescriptorFree(paramDescr, OCI_DTYPE_PARAM);
				});

				const auto fi = descr.buildFiledInfo(paramDescr);
				auto field = generateSelectionField(fi);
				fields.push_back(field);
			}

		} while (status == OCI_SUCCESS);

		return fields;
	}

	FieldDefSp Selector::generateSelectionField(const Describer::field_info_t &fi) const {
		using FDN = FieldDefSpec<OCINumber, SQLT_VNU>;
		using FDS = FieldDefSpec<OCIString *, SQLT_VST>;
		using FDD = FieldDefSpec<OCIDate, SQLT_DAT>;

		FieldDefSp sp;

		switch (fi.dt_) {
			case SQLT_NUM: {
				sp = std::make_shared<FDN>(conn_, fi.strName_);
				break;
			}
			case SQLT_CHR:
			case SQLT_AFC:
			case SQLT_AVC: {
				sp = std::make_shared<FDS>(conn_, fi.strName_, fi.sz_ + 10);
				break;
			}
			case SQLT_DAT: {
				sp = std::make_shared<FDD>(conn_, fi.strName_);
				break;
			}
			default: {
				const std::string strMsg(Poco::format("Unsupported data type: %hu (column: %s)", fi.dt_, fi.strName_));
				throw std::runtime_error(strMsg);
			}
		}

		return sp;
	}

	void Selector::defineFields(Ora::StatementSp &stmt, Rowset::fields_t &fields) const {
		const Connection &conn = *(conn_.get());

		int definePos(1);
		for (auto &fieldSp : fields)
			_OraCall(conn, DefineByPos(conn, stmt, fieldSp, definePos++));

	}

	std::string SelectStatementGenerator::generateSelectStmt(const std::string &strTableName) const {
		std::string strSelList(generateSelectionList(strTableName));
		std::string strStmt(Poco::format("SELECT %s FROM %s", strSelList, strTableName));

		return strStmt;
	}

	std::string SelectStatementGenerator::generateSelectionList(const std::string &strTableName) const {
		std::string strSelList;

		const Connection &conn = *(conn_.get());
		const ErrorSp &spErr = conn.getError();

		DescribeSp descrSp(std::make_shared<Describe>(
				ocihwp::HandleFactory<OCIDescribe>::allocate(conn.getEnv()->get())));

		_OraCall(conn,
				 OCIDescribeAny(conn.getSvcCtx()->get(), spErr->get(),
								cast_vptr(strTableName.c_str()), static_cast<ub4>(strTableName.size()),
								OCI_OTYPE_NAME, 0, OCI_PTYPE_TABLE, descrSp->getRef()));

		OCIParam *pAttrParam = nullptr;

		_OraCall(conn,
				 OCIAttrGet(descrSp->get(), OCI_HTYPE_DESCRIBE,
							&pAttrParam, nullptr, OCI_ATTR_PARAM, spErr->get()));

		uword columnsNum(0);
		_OraCall(conn,
				 OCIAttrGet(pAttrParam, OCI_DTYPE_PARAM,
							&columnsNum, nullptr, OCI_ATTR_NUM_COLS, spErr->get()));

		OCIParam *pColumns = nullptr;
		_OraCall(conn,
				 OCIAttrGet(pAttrParam, OCI_DTYPE_PARAM,
							&pColumns, nullptr, OCI_ATTR_LIST_COLUMNS, spErr->get()));

		Describer descr(conn_);

		for (int i = 0; i < columnsNum; i++) {
			OCIParam *paramColumn = nullptr;
			const auto status = OCIParamGet(pColumns, OCI_DTYPE_PARAM,
											spErr->get(), reinterpret_cast<void **>(&paramColumn), i + 1);
			if (status == OCI_SUCCESS) {
				scope_t<> scope([paramColumn](bool) noexcept {
					OCIDescriptorFree(paramColumn, OCI_DTYPE_PARAM);
				});

				const auto fi = descr.buildFiledInfo(paramColumn);
				std::string strSelElement(fi.strName_);

				if (fi.dt_ == SQLT_DAT) {
					std::string str(Poco::format("TO_CHAR(%s, 'YYYY-MM-DD') as %s", strSelElement, strSelElement));
					strSelElement.swap(str);
				}

				strSelList += strSelElement;
				if (i < (columnsNum - 1))
					strSelList += ", ";
			}
		}

		return strSelList;
	}

}; // namespace Ora
