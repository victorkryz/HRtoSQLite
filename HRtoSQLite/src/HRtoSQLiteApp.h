/**
 * HRtoSQLite
 *
 * @author Victor Kryzhanivskyi
 */

#ifndef HRtoSQLiteWHOLE_HRtoSQLiteAPP_H
#define HRtoSQLiteWHOLE_HRtoSQLiteAPP_H


#include "Poco/Util/Application.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Util/AbstractConfiguration.h"
#include "Poco/AutoPtr.h"
#include "Poco/Util/Validator.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "Poco/File.h"
#include "Poco/String.h"
#include "ExportProcess.h"


using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Util::AbstractConfiguration;
using Poco::Util::OptionCallback;


class HRtoSQLiteApp : public Application
{
    std::ofstream ofs;

public:
    HRtoSQLiteApp() : _helpRequested(false){
        setUnixOptions(true);
    }

protected:
    int main(const ArgVec& args);

    void initialize(Application& self);
    void uninitialize();
    void reinitialize(Application& self);

    void defineOptions(OptionSet& options);
    void handleHelp(const std::string& name, const std::string& value);
    void handleSQLiteDDLFile(const std::string& name, const std::string& value);
    void handleResultFile(const std::string& name, const std::string& value);
    void handleOraConnStr(const std::string& name, const std::string& value);
    void handleOraName(const std::string& name, const std::string& value);
    void handleOraPassword(const std::string& name, const std::string& value);
    void handleDefine(const std::string& name, const std::string& value);
    void displayHelp();
    void defineProperty(const std::string& def);
    void printProperties(const std::string& base);

private:
    bool _helpRequested;
};


#endif //HRtoSQLiteWHOLE_HRtoSQLiteAPP_H
