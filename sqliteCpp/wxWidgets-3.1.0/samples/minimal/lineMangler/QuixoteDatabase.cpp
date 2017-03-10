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


bool QuixoteDatabase::Commit()
{
	// Load the data :) 
	lineMangler::LineLoader loader;
	loader.LoadFiles();
	auto lines = loader.GetLanguageLines(lineMangler::LanguageCode::English);
	auto currentLine = lines.begin();

	for (int i = 0; i < m_lines; ++i)
	{
		string stringId = GenerateNewStringID();
		lineMangler::AddSourceLine a(m_connection,
			stringId,
			*currentLine,
			1,	// Hardcoded Platform (Common by design) 
			0);	// Hardcoded Status
		a.Commit();	// Super stupid implementation

		for (int j = 0; j < m_versions; ++j)
		{
			// Now the Versions 
			lineMangler::ModifySourceLine addVersion(m_connection,
				stringId,
				*currentLine);
			addVersion.Commit();
		}

		currentLine++;
		if (currentLine == lines.end())
			currentLine = lines.begin();
	}
	
	//try
	//{
	//	// Find the SourceLine ID
	//	Statement getSourceLineId = Statement(m_connection, "select SourceLineId from SourceLines where StringId=(?1)", m_stringID);
	//	if (!getSourceLineId.Step())
	//	{
	//		m_lastErrorMessage = "String ID " + m_stringID + " not found";
	//		return false;
	//	}
	//	int sourceLineId = getSourceLineId.GetInt(0);

	//	Statement getCurrentLineContent = Statement(m_connection, "select SourceLineContentId, Version, Text from SourceLineContents where SourceLineId=(?1)", sourceLineId);
	//	getCurrentLineContent.Step();

	//	// Push previous line to historic
	//	Execute(m_connection, "begin");
	//	Statement insert(m_connection, "insert into SourceLineContentsHistory(Version, Text, SourceLineContentId) values(?1, ?2, ?3)", 
	//		getCurrentLineContent.GetInt(1),
	//		getCurrentLineContent.GetString(2),
	//		getCurrentLineContent.GetInt(0));
	//	insert.Execute();

	//	Statement updateCurrentContent(m_connection, "UPDATE SourceLineContents SET Version=(?1), Text=(?2) WHERE SourceLineContentId=(?3)",
	//		getCurrentLineContent.GetInt(1) + 1,
	//		m_text,
	//		getCurrentLineContent.GetInt(0));
	//	updateCurrentContent.Execute();

	//	Execute(m_connection, "commit");
	//}
	//catch (sqliteWrapped::Exception const & e)
	//{
	//	Execute(m_connection, "rollback");
	//	m_lastErrorMessage = e.Message;
	//	return false;
	//}
	return true;
}


string const & QuixoteDatabase::ErrorMessage()
{
	return m_lastErrorMessage;
}