/**
 * HRtoSQLite
 *
 * @author Victor Kryzhanivskyi
 */

#ifndef _export_process__h_
#define _export_process__h_

#include "Interfaces.h"
#include "map"
#include <future>

/**
 * ExportrProcessImpl class implements the export process.
 * It launches the process, handles breaking events,
 * performs reporting about phases, does logging, and etc.
 * It is implemented as a singleton.
 */
class ExportrProcess : public ParameterizedProcess,
									IdleCallback
{
	public:
		class cancel_occurrence : public std::runtime_error
		{
			public:
				explicit cancel_occurrence(const std::string& strMsg) :
					std::runtime_error(strMsg) {};
				explicit cancel_occurrence(const char* strMsg) :
					std::runtime_error(strMsg) {};
				cancel_occurrence(const cancel_occurrence& other) :
					std::runtime_error(other) {};
		};

	public:
		static ExportrProcess& getInstance()
		{
			static ExportrProcess process;
			return process;
		}

		void init(std::ostream* reportStream) override;
		void uninit() override {};

		void startProcess() override;
		int getResultCode() override {
			return resultCode_;
		}

		void setParameter(Parameters param, const std::string& value) override {
			params_[param] = value;
		}
		std::string getParameter(Parameters param) override {
			return obtainParam(param);
		}

	protected:
	    void onIdle() override;
		void onBeforeTableExport(const std::string& stTableName);
		void onAfterTableExport(const std::string& stTableName);

		ExportrProcess();
		virtual ~ExportrProcess() {
		};

		void reinitSQLiteDbStruct();
		void launchTheCycle(std::promise<bool>& prom);
		void exportTable(const std::string strTableName);

		std::string obtainParam(Parameters param)
		{
			auto it = params_.find(param);
			return (it != params_.end()) ?
				params_[param] : std::string();
		}

		std::map<Parameters, std::string> params_;
		std::vector<std::string> tablesToExport_;
		std::string strSQLiteStructInitFile_,
					strSQLiteResultFile_,
					strOracleConnStr_,
					strOracleConnName_,
					strOracleConnPsw; 
		Ora::ConnectionSp spConn_;
		std::ostream* reportStream_ = nullptr;
		int resultCode_ = 0;
};




#endif //_export_process__h_