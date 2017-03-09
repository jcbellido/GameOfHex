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
		static sqliteWrapped::Connection GetConnection(ILogger &logger);
		static sqliteWrapped::Connection GetConnection();
	private:
		static sqliteWrapped::Connection OpenExistingDb(std::wstring const & filePath, ILogger &logger);
		static sqliteWrapped::Connection CreateNewDb(std::wstring const & filePath, ILogger &logger);
		static sqliteWrapped::Connection OpenExistingDb(std::wstring const & filePath);
		static sqliteWrapped::Connection CreateNewDb(std::wstring const & filePath);
	};
}