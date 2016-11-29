/**
 * HRtoSQLite
 *
 * @author Victor Kryzhanivskyi
 */

#ifndef _cmd_file_parser_h
#define _cmd_file_parser_h

#include <string>
#include <list>


namespace Sqlt
{

	/**
	 * StmtFileParser class is a simple parser of a file with
	 * statements delimited by ";" and creates list of ones;
	 */
	class StmtFileParser
	{
	public:
		/**
		 * Class constructor:
		 * @param strFilePath  specifies file path
		 */
		StmtFileParser(const std::string& strFilePath) : strFilePath_(strFilePath) {
		}

		void parse();

		const std::list<std::string>& getStatements() {
			return stmts_;
		}

	private:
		std::string strFilePath_;
		std::list<std::string> stmts_;
	};

}; // Sqlt


#endif //_cmd_file_parser_h