/**
 * HRtoSQLite
 *
 * @author Victor Kryzhanivskyi
 */

#ifndef _hrtosqlt_interfaces_h_
#define _hrtosqlt_interfaces_h_

#include "Ora/Utils.h"

class ParameterizedProcess
{
	public:
		enum class Parameters
		{
			OraConnStr,
			OraConnName,
			OraConnPsw,
			SqltStructFile,
			SqltResulFile
		};

	public:	
		virtual void init(std::ostream* reportStream) = 0;
		virtual void uninit() = 0;
		virtual void startProcess() = 0;
		virtual int getResultCode() = 0;

		virtual void setParameter(Parameters param, const std::string& value) = 0;
		virtual std::string getParameter(Parameters param) = 0;

	protected:
		virtual ~ParameterizedProcess() {}
};


class IdleCallback
{
	public:
		virtual void onIdle() = 0;

	protected:
		virtual ~IdleCallback() {}
};


class RowValuesListener
{
	public:
		using FDNumber = FieldDefSpec<OCINumber, SQLT_VNU>;
		using FDString = FieldDefSpec<OCIString*, SQLT_VST>;
		using FDDate = FieldDefSpec<OCIDate, SQLT_DAT>;
		using FDDateTime = FieldDefSpec<OCIDateTime*, SQLT_TIMESTAMP>;
		using FDSet = std::vector<FieldDefSp>;

		virtual void onIterationBegin(const FDSet& fields) = 0;
		virtual void onIterationEnd() = 0;
		virtual void onRowValue(int fieldIndex, FDNumber& val) = 0;
		virtual void onRowValue(int fieldIndex, FDString& val) = 0;
		virtual void onRowValue(int fieldIndex, FDDate& val) = 0;
		virtual void onRowValue(int fieldIndex, FDDateTime& val) = 0;
		virtual void onRowValue(int fieldIndex, FieldDef& val) = 0;


	protected:
		virtual ~RowValuesListener() {}
};


#endif //_hrtosqlt_interfaces_h_

