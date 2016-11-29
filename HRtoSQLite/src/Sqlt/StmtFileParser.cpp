/**
 * HRtoSQLite
 *
 * @author Victor Kryzhanivskyi
 */

#include "../Common.h"
#include "StmtFileParser.h"

#include "Poco/LineEndingConverter.h"
#include "Poco/FileStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/StreamTokenizer.h"
#include "Poco/StringTokenizer.h"
#include "Poco/File.h"
#include "Poco/Exception.h"
#include "Poco/String.h"
#include <sstream>
#include <iostream>

using Poco::FileInputStream;
using Poco::StreamCopier;
using Poco::StringTokenizer;
using Poco::File;
using Poco::NotFoundException;

namespace Sqlt
{
	void StmtFileParser::parse()
	{
		if (!stmts_.empty())
			return;

		if (!File(strFilePath_).exists())
			throw NotFoundException(Poco::cat(std::string("File "), strFilePath_, std::string(" is not found")));

		FileInputStream fs(strFilePath_);

		std::string tokens;
		StreamCopier::copyToString(fs, tokens);

		StringTokenizer tokenizer(tokens, ";", StringTokenizer::TOK_TRIM);

		std::list<std::string> commands;
		for (StringTokenizer::Iterator it = tokenizer.begin(); it != tokenizer.end(); ++it)
		{
			const std::string& str = *it;
			if (!str.empty())
				commands.push_back(*it);
		}

		fs.close();

		stmts_ = std::move(commands);
	}

}; // namespace Sqlt