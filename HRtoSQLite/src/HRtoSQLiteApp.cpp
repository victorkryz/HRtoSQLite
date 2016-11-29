/**
 * HRtoSQLite
 *
 * @author Victor Kryzhanivskyi
 */

#include "Poco/Util/Application.h"
#include "HRtoSQLiteApp.h"


/**
 * FilePathValidator class validates file path parameter;
 */
class FilePathValidator : public Poco::Util::Validator
{
public:
    virtual void validate(const Option& option, const std::string& value) override
    {
        if (value.empty())
            throw Poco::RuntimeException(Poco::cat(std::string("Parameter value "), option.shortName(), std::string(" is not provided")));

        if (!Poco::File(value).exists())
            throw Poco::NotFoundException(Poco::cat(std::string("File "), value, std::string(" is not found")));
    }
};



void HRtoSQLiteApp::initialize(Application& self)
{
    loadConfiguration(); // load default configuration files, if present
    Application::initialize(self);

    ExportrProcess::getInstance().init(&std::cout);
}

void HRtoSQLiteApp::uninitialize()
{
    ExportrProcess::getInstance().uninit();
    Application::uninitialize();
}

void HRtoSQLiteApp::reinitialize(Application& self) {
    Application::reinitialize(self);
}


int HRtoSQLiteApp::main(const ArgVec& args)
{
    int exitCode(Application::EXIT_OK);

    if (!_helpRequested)
    {
        auto& process = ExportrProcess::getInstance();
        process.startProcess();
        exitCode = process.getResultCode();
    }

    return exitCode;
}


void HRtoSQLiteApp::defineOptions(OptionSet& options)
{
    Application::defineOptions(options);

    options.addOption(
            Option("help", "h", "display help information on command line arguments")
                    .required(false)
                    .repeatable(false)
                    .callback(OptionCallback<HRtoSQLiteApp>(this, &HRtoSQLiteApp::handleHelp)));

    options.addOption(
            Option("ora-conn-str", "C", "Oracle connection string (//host[:port]/service)")
                    .required(true)
                    .repeatable(false)
                    .argument("value")
                    .callback(OptionCallback<HRtoSQLiteApp>(this, &HRtoSQLiteApp::handleOraConnStr)));

    options.addOption(
            Option("ora-name", "N", "Oracle user name (default: \"hr\")")
                    .required(false)
                    .repeatable(false)
                    .argument("value")
                    .callback(OptionCallback<HRtoSQLiteApp>(this, &HRtoSQLiteApp::handleOraName)));

    options.addOption(
            Option("ora-psw", "P", "Oracle user password")
                    .required(false)
                    .repeatable(false)
                    .argument("value")
                    .callback(OptionCallback<HRtoSQLiteApp>(this, &HRtoSQLiteApp::handleOraPassword)));

    options.addOption(
            Option("sqlt-ddl", "S", "HR-SQLite database structute intialization file (default: ./sql/hrSQLiteDDL.sql)")
                    .required(false)
                    .repeatable(false)
                    .argument("file")
                    .validator(new FilePathValidator())
                    .callback(OptionCallback<HRtoSQLiteApp>(this, &HRtoSQLiteApp::handleSQLiteDDLFile)));


    options.addOption(
            Option("output", "O", "resulting SQLite database file (default: ./hr.db)")
                    .required(false)
                    .repeatable(false)
                    .argument("file")
                    .callback(OptionCallback<HRtoSQLiteApp>(this, &HRtoSQLiteApp::handleResultFile)));
}

void HRtoSQLiteApp::defineProperty(const std::string& def)
{
    std::string name;
    std::string value;
    std::string::size_type pos = def.find('=');
    if (pos != std::string::npos)
    {
        name.assign(def, 0, pos);
        value.assign(def, pos + 1, def.length() - pos);
    }
    else name = def;
    config().setString(name, value);
}

void HRtoSQLiteApp::handleHelp(const std::string& name, const std::string& value)
{
    _helpRequested = true;
    displayHelp();
    stopOptionsProcessing();
}

void HRtoSQLiteApp::handleSQLiteDDLFile(const std::string& name, const std::string& value) {
    ExportrProcess::getInstance().setParameter(ExportrProcess::Parameters::SqltStructFile,value);
}

void HRtoSQLiteApp::handleResultFile(const std::string& name, const std::string& value) {
    ExportrProcess::getInstance().setParameter(ExportrProcess::Parameters::SqltResulFile, value);
}

void HRtoSQLiteApp::handleOraConnStr(const std::string& name, const std::string& value) {
    ExportrProcess::getInstance().setParameter(ExportrProcess::Parameters::OraConnStr, value);
}

void HRtoSQLiteApp::handleOraName(const std::string& name, const std::string& value) {
    ExportrProcess::getInstance().setParameter(ExportrProcess::Parameters::OraConnName, value);
}

void HRtoSQLiteApp::handleOraPassword(const std::string& name, const std::string& value) {
    ExportrProcess::getInstance().setParameter(ExportrProcess::Parameters::OraConnPsw, value);
}

void HRtoSQLiteApp::handleDefine(const std::string& name, const std::string& value){
    defineProperty(value);
}

void HRtoSQLiteApp::displayHelp()
{
    HelpFormatter helpFormatter(options());
    helpFormatter.setCommand(commandName());
    helpFormatter.setUsage("parameters");
    helpFormatter.setHeader("Transfer Oracle Database sample HR schema's data into SQLite database.\nParameters are:");
    helpFormatter.format(std::cout);
}

void HRtoSQLiteApp::printProperties(const std::string& base)
{
    AbstractConfiguration::Keys keys;
    config().keys(base, keys);
    if (keys.empty())
    {
        if (config().hasProperty(base))
        {
            std::string msg;
            msg.append(base);
            msg.append(" = ");
            msg.append(config().getString(base));
            logger().information(msg);
        }
    }
    else
    {
        for (AbstractConfiguration::Keys::const_iterator it = keys.begin(); it != keys.end(); ++it)
        {
            std::string fullKey = base;
            if (!fullKey.empty()) fullKey += '.';
            fullKey.append(*it);
            printProperties(fullKey);
        }
    }
}




