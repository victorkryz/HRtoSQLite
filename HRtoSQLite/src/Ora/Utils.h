/**
 * HRtoSQLite
 *
 * @author Victor Kryzhanivskyi
 */

#ifndef _orautil_h
#define _orautil_h

#ifdef _WIN32
	#pragma warning(disable : 4359)
#endif

#include "Types.h"
#include "Connection.h"
#include <vector>


namespace Ora
{
	inline oratext* cast_oratext(const char* pstr) {
		return reinterpret_cast<oratext*>(const_cast<char*>(pstr));
	}

	inline const oratext* cast_const_oratext(const char* pstr) {
		return reinterpret_cast<oratext*>(const_cast<char*>(pstr));
	}

	inline const oratext* cast_const_oratext(const std::string& str) {
		return cast_const_oratext(str.c_str());
	}

	template<typename T> inline void* cast_vptr(const T* p) {
		return reinterpret_cast<void*>(const_cast<T*>(p));
	}


	inline sword extractErrorInfo(OCIError* err, sword statusOfFnk, sb4& code, std::string& msg)
	{
		sword status(0);

		if (OCI_ERROR == statusOfFnk)
		{
			text chBuf[512];

			status = OCIErrorGet(err, (ub4)1, cast_oratext(nullptr),
								&code, chBuf, sizeof(chBuf), (ub4)OCI_HTYPE_ERROR);

			msg.assign((const char*)chBuf);
		}

		return status;
	}

	inline void raiseException(const Connection& ses, sword status)
	{
		std::string strMsg("Exception");

		sb4  sbCode(-1);

		if (OCI_ERROR == status)
		{
			const ErrorSp& spErr = ses.getError();
			extractErrorInfo(spErr->get(), status, sbCode, strMsg);
		}

		throw Ora::Exception(status, sbCode, strMsg);
	}


	inline sword _OraCall(const Connection& ses, sword status)
	{
		if ((OCI_SUCCESS != status) && 
			    (OCI_NO_DATA != status))	 
			raiseException(ses, status); 
		return status;
	}



    inline sword setPrefetch(StatementSp& stmtSp, const ErrorSp& spErr, ub4 cRows, ub4 cbMem)
    {
        auto status = OCIAttrSet(stmtSp->get(), OCI_HTYPE_STMT,
                                 &cRows, 0, OCI_ATTR_PREFETCH_ROWS, spErr->get());
        if ( status == OCI_SUCCESS )
            status = OCIAttrSet(stmtSp->get(), OCI_HTYPE_STMT,
                                &cbMem, 0, OCI_ATTR_PREFETCH_MEMORY, spErr->get());
        return status;
    }



	inline StatementSp stmtExec(const Ora::ConnectionSp& spConn, const std::string& strStmt, bool bSelectStmt = true)
	{
		auto stmtSp = std::make_shared<Ora::Statement>(
				ocihwp::HandleFactory<OCIStmt>::allocate(spConn->getEnv()->get()));

		const auto& conn = *(spConn.get());
		const auto& spErr = conn.getError();

		_OraCall(conn,
				 OCIStmtPrepare(stmtSp->get(), spErr->get(),
								cast_const_oratext(strStmt), static_cast<ub4>(strStmt.length()),
                                OCI_NTV_SYNTAX, OCI_DEFAULT));

        if ( bSelectStmt )
            setPrefetch(stmtSp,  spErr, 10, 10000);

        const ub4 iters(bSelectStmt ? 0 : 1);
		_OraCall(conn,
				 OCIStmtExecute(conn.getSvcCtx()->get(), stmtSp->get(), spErr->get(),
								iters, 0, nullptr, nullptr, OCI_DEFAULT));

		return std::move(stmtSp);
	}

};

class UpdateName
{
public:
	virtual void updateName(std::string& strName) = 0;

	virtual ~UpdateName() {};
};

class FieldDef : public UpdateName
{
	public:
		virtual const std::string& getName() = 0;
		virtual sb4 getDty() = 0;
		virtual ub2 getValSz() = 0;
		virtual void* getRawValPtr() = 0;
		virtual OCIDefine*& getDefine() = 0;
		virtual sb2& getIndicator() = 0;
		virtual ub2& getActualSize() = 0;
		virtual bool isNULL() const = 0;
		virtual bool isNotNULL() const = 0;
	
		virtual ~FieldDef() {};
};

typedef std::shared_ptr<FieldDef> FieldDefSp;

template <typename T, ub2 DTY> 
class alignas(2) FieldDefSpec : public FieldDef
{
	public:
		FieldDefSpec(const Ora::ConnectionSp& conn, ub2 valsize = 0) : conn_(conn),
			valsize_(valsize) 
		{
			init();
			templSpecInit();
		}

		FieldDefSpec(const Ora::ConnectionSp& conn, const std::string& strName, ub2 valsize = 0) :
			FieldDefSpec(conn, valsize) {
			strName_ = strName;
		};

		FieldDefSpec(const FieldDefSpec<T, DTY>& fieldFrom) = delete;

		FieldDefSpec(FieldDefSpec<T, DTY>&& fieldFrom)
		{
			value_ = std::move(fieldFrom.value_);
			strName_ = std::move(fieldFrom.strName_);
			conn_ = fieldFrom.conn_;
			defineSp_ = fieldFrom.defineSp_;
			indicator_ = fieldFrom.indicator_;
			actualSize_ = fieldFrom.actualSize_;
			valsize_ = fieldFrom.valsize_;

			fieldFrom.conn_ = nullptr;
			fieldFrom.indicator_ = 0;
			fieldFrom.actualSize_ = 0;
		}

		~FieldDefSpec() 
		{
			templSpecDeinit();
			deinit();
		}

	protected:
		void init() {
			defineSp_= Ora::DefineSp(std::make_shared<Ora::Define>());
		};
		void templSpecInit() {}
		void deinit() {}
		void templSpecDeinit() {}

	public:
		const std::string& getName() override {
			return strName_;
		}

		sb4 getDty() override {
			return DTY;
		}

		T& getVal() {
			return value_;
		}

		T* getValPtr() {
			return &value_;
		}

		void* getRawValPtr() override {
			return &value_;
		}

		ub2 getValSz() override {
			return sizeof(value_);
		}

		OCIDefine*& getDefine() override {
			return defineSp_->getRef();
		}

		sb2& getIndicator() override {
			return indicator_;
		}

		ub2& getActualSize() override {
			return actualSize_;
		}

		bool isNULL() const override {
			return (indicator_ < 0);
		}

		bool isNotNULL() const override {
			return (indicator_ >= 0);
		}

		void updateName(std::string& strName) override {
			strName_ = strName;
		}

		const Ora::ConnectionSp& getConn() {
			return conn_;
		}

		Ora::ConnectionSp conn_;
		std::string strName_;
		T value_ = T();
		Ora::DefineSp defineSp_;
		sb2 indicator_ = 0;
		ub2 actualSize_ = 0;
		ub4 valsize_ = 0;
};

template <> inline void FieldDefSpec<OCIString*, SQLT_VST>::templSpecInit() {
	OCIStringResize(conn_->getEnv()->get(), conn_->getError()->get(), valsize_, &value_);
}

template <> inline void FieldDefSpec<OCIString*, SQLT_VST>::templSpecDeinit() {
	OCIStringResize(conn_->getEnv()->get(), conn_->getError()->get(), 0, &value_);
}


template <> inline ub2 FieldDefSpec<OCIString*, SQLT_VST>::getValSz() {
	return valsize_; //!!!
}

inline sword DefineByPos(const Ora::Connection& conn, const Ora::StatementSp& spStmt, FieldDefSp& fieldDef, ub4 position)
{
	auto status = _OraCall(conn,
		OCIDefineByPos(spStmt->get(), &fieldDef->getDefine(), conn.getError()->get(), position,
			fieldDef->getRawValPtr(), fieldDef->getValSz(), fieldDef->getDty(),
			&(fieldDef->getIndicator()), &fieldDef->getActualSize(), nullptr, OCI_DEFAULT));
	return status;
}


template <typename T, ub2 DTY, size_t SZ = 0>
sword DefineByPos(const Ora::Connection& conn, const Ora::StatementSp& spStmt, FieldDefSpec<T, DTY>& fieldDef, ub4 position)
{
	auto status = _OraCall(conn,
		OCIDefineByPos(spStmt->get(), &fieldDef.getDefine(), conn.getError()->get(), position,
			&fieldDef.getVal(), fieldDef.getValSz(), fieldDef.getDty(),
			&(fieldDef.getIndicator()), &fieldDef.getActualSize(), nullptr, OCI_DEFAULT));
	return status;
}


#include <cstring>

class DTConv
{
	public:
		DTConv(const Ora::ConnectionSp& conn) : conn_(conn) {
		}

		std::string OCIStrToStdStr(OCIString* srcStr) const
		{
			const Ora::EnvSp& spEnv = conn_->getEnv();
			std::string strTmp;

			ub4 szStr = OCIStringSize(spEnv->get(), srcStr);
			if (0 != szStr)
			{
				oratext* p = OCIStringPtr(spEnv->get(), srcStr);
				strTmp.assign(reinterpret_cast<char*>(p), szStr);
			}

			return std::move(strTmp);
		}

		bool OCINumberIsInt(const OCINumber& num) const
		{
			const Ora::ErrorSp& spErr = conn_->getError();

			boolean bIsInt(0);
			_OraCall(*(conn_.get()),
				::OCINumberIsInt(spErr->get(), &num, &bIsInt));

			return 1 == bIsInt;
		}

		template <typename T> void OCINumberToInt(const OCINumber& num, T& resVal, bool bSigned) const
		{

			const Ora::ErrorSp& spErr = conn_->getError();
				_OraCall(*(conn_.get()),
						::OCINumberToInt(spErr->get(), &num, sizeof(T),
										bSigned ? OCI_NUMBER_SIGNED : OCI_NUMBER_UNSIGNED,  &resVal));
		}


		template <typename T> void OCINumberToReal(const OCINumber& num, T& resVal, bool bSigned) const
		{
			const Ora::ErrorSp& spErr = conn_->getError();
			_OraCall(*(conn_.get()),
						::OCINumberToReal(spErr->get(), &num, sizeof(T), &resVal));
		}

		std::string OCIDateToStdStr(const OCIDate& date, 
									std::string strFormat = std::string(), std::string strLang = std::string()) const
		{
			const Ora::EnvSp& spEnv = conn_->getEnv();
			const Ora::ErrorSp& spErr = conn_->getError();

			OraText chBuf[200] = {0,0};
			ub4 bufSz(sizeof(chBuf));

			const OraText* pFormat = strFormat.empty() ? nullptr :  
														reinterpret_cast<const OraText*>(strFormat.c_str());
			ub1 formatSz(static_cast<ub1>(strFormat.size()));

			const OraText* pLang = strLang.empty() ? nullptr :
													reinterpret_cast<const OraText*>(strLang.c_str());
			ub1 langSz(static_cast<ub1>(strLang.size()));

			_OraCall(*(conn_.get()),
					::OCIDateToText(spErr->get(), &date, 
									pFormat, formatSz,
									pLang, langSz,
									&bufSz, chBuf));
			std::string strTmp;
			if (0 != bufSz)
				strTmp.assign(reinterpret_cast<char*>(chBuf), bufSz);

			return std::move(strTmp);
		}


		std::string OCIDateTimeToStdStr(OCIDateTime* date,
			std::string strFormat = std::string(), std::string strLang = std::string()) const
		{
			const Ora::EnvSp& spEnv = conn_->getEnv();
			const Ora::ErrorSp& spErr = conn_->getError();
			const Ora::SessionSp& spSes = conn_->getSession();

			OraText chBuf[200] = { 0,0 };
			ub4 bufSz(sizeof(chBuf));

			const OraText* pFormat = strFormat.empty() ? nullptr :
				reinterpret_cast<const OraText*>(strFormat.c_str());
			ub1 formatSz(static_cast<ub1>(strFormat.size()));

			const OraText* pLang = strLang.empty() ? nullptr :
				reinterpret_cast<const OraText*>(strLang.c_str());
			ub1 langSz(static_cast<ub1>(strLang.size()));

			_OraCall(*(conn_.get()),
				::OCIDateTimeToText(spEnv->get(), spErr->get(), date,
									pFormat, formatSz, 9,
									pLang, langSz,
									&bufSz, chBuf));
			std::string strTmp;
			if (0 != bufSz)
				strTmp.assign(reinterpret_cast<char*>(chBuf), bufSz);

			return std::move(strTmp);
		}

		
		const Ora::ConnectionSp conn_;
};




#endif // _orautil_h

