#include "QuixoteDatabase.h"

#include <chrono>

#include "lineMangler\LineMangler.h"
#include "AddSourceLine.h"

using namespace lineMangler;
using namespace sqliteWrapped;

string QuixoteDatabase::GenerateNewStringID()
{
	chrono::system_clock::time_point now = chrono::system_clock::now();
	auto microSeconds = now.time_since_epoch().count();
	std::string stringId = std::to_string(static_cast<unsigned long long>(microSeconds));
	return stringId;
}

bool QuixoteDatabase::Queue()
{
	return Commit();
}

bool QuixoteDatabase::Commit()
{
	// Load the data :) 
	lineMangler::LineLoader loader;
	loader.LoadFiles();
	auto lines = loader.GetLanguageLines(lineMangler::LanguageCode::English);
	auto currentLine = lines.begin();

	sqliteWrapped::Execute(m_connection, "PRAGMA synchronous=OFF");
	//Statement pragmaMemory (m_connection, "PRAGMA journal_mode=MEMORY");
	//pragmaMemory.Step();

	const int HARD_LIMIT = 4096;
	int currentWrittingNumber = 0;
	sqliteWrapped::Execute(m_connection, "begin transaction");
	for (int i = 0; i < m_lines; ++i)
	{
		string stringId = GenerateNewStringID();
		lineMangler::AddSourceLine a(m_connection,
			stringId,
			*currentLine,
			1,	// Hardcoded Platform (Common by design) 
			0);	// Hardcoded Status
		a.Queue();

		for (int j = 0; j < m_versions; ++j)
		{
			// Now the Versions 
			lineMangler::ModifySourceLine addVersion(m_connection,
				stringId,
				*currentLine);
			addVersion.Queue();
		}

		currentLine++;
		if (currentLine == lines.end())
			currentLine = lines.begin();
		
		currentWrittingNumber++;
		if (HARD_LIMIT <= currentWrittingNumber)
		{
			sqliteWrapped::Execute(m_connection, "end transaction");
			sqliteWrapped::Execute(m_connection, "begin transaction");
			currentWrittingNumber = 0;
		}
	}
	sqliteWrapped::Execute(m_connection, "end transaction");
	sqliteWrapped::Execute(m_connection, "PRAGMA synchronous=ON");
	sqliteWrapped::Execute(m_connection, "analyze");
	//Statement pragmaDelete(m_connection, "PRAGMA journal_mode=DELETE");
	//pragmaDelete.Step();

	return true;
}


string const & QuixoteDatabase::ErrorMessage()
{
	return m_lastErrorMessage;
}