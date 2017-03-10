#include <Windows.h>

#include "connectionFactory.h"

// First implementation based on the frame ... which makes like ... zero sense
sqliteWrapped::Connection lineMangler::ConnectionFactory::GetConnection(ILogger &logger)
{
	logger.AddToLog(L"Connection factory called");
	auto executablePath = sillyUtils::GetExecutablePath();
	executablePath += L".db";
	logger.AddToLog(executablePath);

	if (sillyUtils::is_file_exist(executablePath))
		return ConnectionFactory::OpenExistingDb(executablePath, logger);
	else
		return ConnectionFactory::CreateNewDb(executablePath, logger);
}

sqliteWrapped::Connection lineMangler::ConnectionFactory::OpenExistingDb(std::wstring const & filePath, ILogger &logger)
{
	auto connection = sqliteWrapped::Connection(filePath.c_str());
	logger.AddToLog(L"File exists, Schema not created");
	return connection;
}

sqliteWrapped::Connection lineMangler::ConnectionFactory::CreateNewDb(std::wstring const & filePath, ILogger &logger)
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
		return sqliteWrapped::Connection::WideMemory();
	}
}

sqliteWrapped::Connection lineMangler::ConnectionFactory::GetConnection()
{
	auto executablePath = sillyUtils::GetExecutablePath();
	executablePath += L".db";

	if (sillyUtils::is_file_exist(executablePath))
		return ConnectionFactory::OpenExistingDb(executablePath);
	else
		return ConnectionFactory::CreateNewDb(executablePath);
}

sqliteWrapped::Connection lineMangler::ConnectionFactory::OpenExistingDb(std::wstring const & filePath)
{
	auto connection = sqliteWrapped::Connection(filePath.c_str());
	return connection;
}

sqliteWrapped::Connection lineMangler::ConnectionFactory::CreateNewDb(std::wstring const & filePath)
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

		auto newDatabase = sqliteWrapped::Connection(filePath.c_str());
		return newDatabase;
	}
	catch (sqliteWrapped::Exception const & e)
	{
		return sqliteWrapped::Connection::WideMemory();
	}
}
