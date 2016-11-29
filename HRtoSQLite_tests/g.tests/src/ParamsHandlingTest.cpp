/**
 * HRtoSQLite_tests
 *
 * @author Victor Kryzhanivskyi
 */

#include <stdio.h>
#include <string>
#include "gtest/gtest.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/OptionProcessor.h"
#include "Poco/Util/OptionException.h"

class ParamsHandling
{
	public:
		ParamsHandling(){			
		}
		~ParamsHandling() {
		}

		void testDefault()
		{
			Poco::Util::OptionSet set;
			set.addOption(
				Poco::Util::Option("include-dir", "I", "specify a search path for locating header files")
				.required(false)
				.repeatable(true)
				.argument("path"));

			set.addOption(
				Poco::Util::Option("ora-conn-str", "R", "defines an Oracle connection string")
				.required(true)
				.repeatable(false)
				.argument("value"));
		}
};


TEST(ParamsHandling, CommandLineProcessing)
{
	using Poco::Util::Option;
	Poco::Util::OptionSet set;

	try
	{
		set.addOption(
			Poco::Util::Option("ora-conn-str", "R", "defines an Oracle connection string")
			.required(true)
			.repeatable(false)
			.argument("value"));

		set.addOption(
			Option("sqlt-ddl", "S", "defines a HR-SQLite structute intialization file (default: .\\sql\\hrSQLiteDDL.sql)")
			.required(false)
			.repeatable(false)
			.argument("file"));

		EXPECT_TRUE(set.hasOption("sqlt-ddl", false));
		EXPECT_TRUE(set.hasOption("S", true));

		EXPECT_TRUE(set.hasOption("ora-conn-str", false));
		EXPECT_TRUE(set.hasOption("R", true));

		std::string name, value;

		{
			Poco::Util::OptionProcessor p(set);
			p.setUnixStyle(true);

			EXPECT_TRUE(p.process("-R//localhost:1521/xe", name, value));
			EXPECT_STREQ(name.c_str(), "ora-conn-str");
			EXPECT_STREQ(value.c_str(), "//localhost:1521/xe");
		}

		{
			Poco::Util::OptionProcessor p(set);
			p.setUnixStyle(true);

			EXPECT_TRUE(p.process("--ora-conn-str://remotehost:1521/xe", name, value));
			EXPECT_STREQ(name.c_str(), "ora-conn-str");
			EXPECT_STREQ(value.c_str(), "//remotehost:1521/xe");
		}
	}

	catch (const Poco::Util::UnknownOptionException& e)
	{
		std::string str(e.what());
		std::cout << "UnknownOptionException:" << str << std::endl;
		throw;
	}

	catch (const Poco::Util::DuplicateOptionException& e)
	{
		std::string str(e.what());
		std::cout << "DuplicateOptionException:" << str << std::endl;
		throw;
	}

	catch (const std::exception& e)
	{
		std::string str(e.what());
		std::cout << str << e.what() << std::endl;
		throw;
	}
};

/*
TEST(ParamsHandling, WinxStyle)
{
	using Poco::Util::Option;
	Poco::Util::OptionSet set;

	try
	{

		set.addOption(
			Poco::Util::Option("ora-conn-str", "R", "defines an Oracle connection string")
			.required(true)
			.repeatable(false)
			.argument("value"));

		set.addOption(
			Option("sqlt-ddl", "S", "defines a HR-SQLite structute intialization file (default: .\\sql\\hrSQLiteDDL.sql)")
			.required(false)
			.repeatable(false)
			.argument("file"));

		EXPECT_TRUE(set.hasOption("sqlt-ddl", false));
		EXPECT_TRUE(set.hasOption("S", true));

		EXPECT_TRUE(set.hasOption("ora-conn-str", false));
		EXPECT_TRUE(set.hasOption("R", true));

		Poco::Util::OptionProcessor p1(set);
		p1.setUnixStyle(false);

		std::string name;
		std::string value;

//		EXPECT_TRUE(p1.process("/R//localhost:1521/xe", name, value));
		EXPECT_TRUE(p1.process("/R", name, value));
		EXPECT_STREQ(name.c_str(), "ora-conn-str");
		EXPECT_STREQ(value.c_str(), "//localhost:1521/xe");

		Poco::Util::OptionProcessor p2(set);
		p2.setUnixStyle(false);

		EXPECT_TRUE(p2.process("--ora-conn-str://remotehost:1521/xe", name, value));
		EXPECT_STREQ(name.c_str(), "ora-conn-str");
		EXPECT_STREQ(value.c_str(), "//remotehost:1521/xe");
	}

	catch (const Poco::Util::UnknownOptionException& e)
	{
		std::string str(e.what());
		std::cout << "UnknownOptionException:" << str << std::endl;
		throw;
	}

	catch (const Poco::Util::DuplicateOptionException& e)
	{
		std::string str(e.what());
		std::cout << "DuplicateOptionException:" << str << std::endl;
		throw;
	}

	catch (const std::exception& e)
	{
		std::string str(e.what());
		std::cout << str << e.what() << std::endl;
		throw;
	}
};
*/