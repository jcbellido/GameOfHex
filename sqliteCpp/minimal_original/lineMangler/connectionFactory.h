#pragma once

#include <iostream>

#include "sqliteWrapped\sqliteWrapped.h"
#include "sillyUtils.h"
#include "Logger.h"

namespace lineMangler
{
	class ConnectionFactory
	{
	public: 
		/// Just in case you have any logger available at the creation time
		static sqliteWrapped::Connection GetConnection(ILogger &logger);
		
		/// Common on start connection creation
		static sqliteWrapped::Connection GetConnection();

	private:
		static sqliteWrapped::Connection OpenExistingDb(std::wstring const & filePath, ILogger &logger);
		static sqliteWrapped::Connection CreateNewDb(std::wstring const & filePath, ILogger &logger);
		static sqliteWrapped::Connection OpenExistingDb(std::wstring const & filePath);
		static sqliteWrapped::Connection CreateNewDb(std::wstring const & filePath);

		static void ProfileQueryTookTooLong(void *, char const * const statement, unsigned long long const time);
	};
}