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
		static sqliteWrapped::Connection GetConnection(ILogger &logger)
		{
			logger.AddToLog(L"Connection factory called");
			auto executablePath = sillyUtils::GetExecutablePath();
			executablePath += L".db";
			logger.AddToLog(executablePath);

			if (sillyUtils::is_file_exist(executablePath))
				return OpenExistingDb(executablePath, logger);
			else
				return CreateNewDb(executablePath, logger);
		}
	private:
		static sqliteWrapped::Connection OpenExistingDb(std::wstring const & filePath, ILogger &logger)
		{
			auto connection = sqliteWrapped::Connection(filePath.c_str());
			logger.AddToLog(L"File exists, Schema not created");
			return connection;
		}

		static sqliteWrapped::Connection CreateNewDb(std::wstring const & filePath, ILogger &logger)
		{
			try
			{
				auto command = L"/C " + sillyUtils::GetExecutableDir() + L"\\sqlite.exe " + filePath;
				command += L" < " + sillyUtils::GetExecutableDir() + L"\\schema.txt ";
				logger.AddToLog(L"No .db file found, loading schema.txt");
				logger.AddToLog(L"Executing external command: " + command);
				ShellExecute(0, L"open", L"cmd.exe", command.c_str(), 0, SW_SHOW);
				auto newDatabase = sqliteWrapped::Connection(filePath.c_str());
				return newDatabase;
			}
			catch (sqliteWrapped::Exception const & e)
			{
				logger.AddToLog(e.Message);
				return sqliteWrapped::Connection::WideMemory();
			}
		}
	};
}