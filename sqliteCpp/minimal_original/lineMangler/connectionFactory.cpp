#include <Windows.h>

#include "connectionFactory.h"
#include "sillyUtils.h"

#include <iostream>

using namespace lineMangler;

// First implementation based on the frame ... which makes like ... zero sense
sqliteWrapped::Connection ConnectionFactory::GetConnection(ILogger &logger)
{
	logger.AddToLog(L"Connection factory called");
	auto executablePath = sillyUtils::GetExecutablePath();
	executablePath += L".db";
	logger.AddToLog(executablePath);

	sqliteWrapped::Connection conn; 

	if (sillyUtils::is_file_exist(executablePath))
		conn = ConnectionFactory::OpenExistingDb(executablePath, logger);
	else
		conn = ConnectionFactory::CreateNewDb(executablePath, logger);

	sqliteWrapped::Execute(conn, "PRAGMA foreign_keys=on;");
	conn.Profile(ProfileQueryTookTooLong);
	return conn;
}

sqliteWrapped::Connection ConnectionFactory::OpenExistingDb(std::wstring const & filePath, ILogger &logger)
{
	auto connection = sqliteWrapped::Connection(filePath.c_str());
	logger.AddToLog(L"File exists, Schema not created");
	return connection;
}

sqliteWrapped::Connection ConnectionFactory::CreateNewDb(std::wstring const & filePath, ILogger &logger)
{
	try
	{
		auto baseCommand = L"/C " + sillyUtils::GetExecutableDir() + L"\\sqlite.exe " + filePath;
		auto command = baseCommand + L" < " + sillyUtils::GetExecutableDir() + L"\\schema.txt ";
		logger.AddToLog(L"No .db file found, loading schema.txt");
		logger.AddToLog(L"Executing external command: " + command);
		ShellExecute(0, L"open", L"cmd.exe", command.c_str(), 0, SW_SHOW);

		Sleep(1500); // see note below
		command = baseCommand + L" < " + sillyUtils::GetExecutableDir() + L"\\LanguagesAndPlatforms.txt ";
		ShellExecute(0, L"open", L"cmd.exe", command.c_str(), 0, SW_SHOW);

		auto newDatabase = sqliteWrapped::Connection(filePath.c_str());
		return newDatabase;
	}
	catch (sqliteWrapped::Exception const & e)
	{
		logger.AddToLog(e.Message);
		return sqliteWrapped::Connection::Memory();
	}
}

sqliteWrapped::Connection ConnectionFactory::GetConnection()
{
	auto executablePath = sillyUtils::GetExecutablePath();
	executablePath += L".db";

	sqliteWrapped::Connection conn;

	if (sillyUtils::is_file_exist(executablePath))
		conn = ConnectionFactory::OpenExistingDb(executablePath);
	else
		conn = ConnectionFactory::CreateNewDb(executablePath);

	sqliteWrapped::Execute(conn, "PRAGMA foreign_keys=on;");
	conn.Profile(ProfileQueryTookTooLong);
	return conn;
}

sqliteWrapped::Connection ConnectionFactory::OpenExistingDb(std::wstring const & filePath)
{
	auto connection = sqliteWrapped::Connection(sillyUtils::ConvertToStandard(filePath).c_str());
	return connection;
}

sqliteWrapped::Connection ConnectionFactory::CreateNewDb(std::wstring const & filePath)
{
	try
	{
		auto baseCommand = L"/C " + sillyUtils::GetExecutableDir() + L"\\sqlite.exe " + filePath;
		// First the schema
		auto command = baseCommand + L" < " + sillyUtils::GetExecutableDir() + L"\\schema.txt ";
		ShellExecute(0, L"open", L"cmd.exe", command.c_str(), 0, SW_SHOW);

		Sleep(1500);	// I really don't want to implement a call to ShellExecutEx, in my shitty machine this takes less than half a second ... 
		 
		// Base config data: Languages and Platforms
		command = baseCommand + L" < " + sillyUtils::GetExecutableDir() + L"\\LanguagesAndPlatforms.txt ";
		ShellExecute(0, L"open", L"cmd.exe", command.c_str(), 0, SW_SHOW);

		auto newDatabase = sqliteWrapped::Connection(sillyUtils::ConvertToStandard(filePath).c_str());

		return newDatabase;
	}
	catch (sqliteWrapped::Exception)
	{
		return sqliteWrapped::Connection::WideMemory();
	}
}

void ConnectionFactory::ProfileQueryTookTooLong(void *, char const * const statement, unsigned long long const time)
{
	unsigned long long const ms = time / 1000000;
	if (ms > 10)
	{
		std::cerr << "Took too long (" << ms << ") " << statement << std::endl;
	}
}