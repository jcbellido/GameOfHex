#pragma once

#include "sqliteWrapped\sqliteWrapped.h"
#include "sillyUtils.h"
#include "Logger.h"

namespace lineMangler
{
	class ConnectionFactory
	{
	public: 
		static sqliteWrapped::Connection GetConnection(ILogger &logger)
		{
			logger.AddToLog(L"Connection factory called");
			auto executablePath = sillyUtils::GetExecutablePath();
			executablePath += L".db";
			logger.AddToLog(executablePath);

			if (sillyUtils::is_file_exist(executablePath))
			{
				auto connection = sqliteWrapped::Connection(executablePath.c_str());
				logger.AddToLog(L"File exists, Schema not created");
				return connection;
			}
			auto newDatabase = sqliteWrapped::Connection(executablePath.c_str());
			logger.AddToLog(L"New file, creating schema from scratch");
			return newDatabase;
		}
	private:


	};
}