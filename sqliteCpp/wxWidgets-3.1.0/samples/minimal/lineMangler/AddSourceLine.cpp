#include "AddSourceLine.h"

using namespace lineMangler;
using namespace sqliteWrapped;



bool AddSourceLine::Queue()
{
	try
	{
		InternalAddSourceLine();
	}
	catch (sqliteWrapped::Exception const & e)
	{
		m_lastErrorMessage = e.Message;
		return false;
	}
	return true;
}

void AddSourceLine::InternalAddSourceLine()
{
	Statement insert(m_connection, "insert into SourceLines(StringId, Platform, Status) values(?1, ?2, ?3)", m_stringID, m_platform, m_status);
	insert.Execute();

	// Keeping the FK reference
	long long rowId = m_connection.RowId();
	Statement checkId = Statement(m_connection, "select SourceLineId from SourceLines where rowId=(?1)", static_cast<int>(rowId));
	checkId.Step();
	int lastIdInserted = checkId.GetInt(0);

	Statement insertContents(m_connection, "insert into SourceLineContents(Version, Text, SourceLineId) values(?1, ?2, ?3)", 0, m_text, lastIdInserted);
	insertContents.Execute();

}

bool AddSourceLine::Commit() 
{
	try
	{
		Execute(m_connection, "begin");
		InternalAddSourceLine();
		Execute(m_connection, "commit");
	}
	catch (sqliteWrapped::Exception const & e)
	{
		Execute(m_connection, "rollback");
		m_lastErrorMessage = e.Message;
		return false;
	}
	return true;
}

string const & AddSourceLine::ErrorMessage()
{
	return m_lastErrorMessage;
}



// ----------------------------------------------------------------------
// Modify existing Source Line Content

bool ModifySourceLine::Queue()
{
	try
	{
		InternalModifySourceLine();
	}
	catch (sqliteWrapped::Exception const & e)
	{
		m_lastErrorMessage = e.Message;
		return false;
	}
	return true;
}

void ModifySourceLine::InternalModifySourceLine()
{
	// Find the SourceLine ID
	Statement getSourceLineId = Statement(m_connection, "select SourceLineId from SourceLines where StringId=(?1)", m_stringID);
	if (!getSourceLineId.Step())
	{
		m_lastErrorMessage = "String ID " + m_stringID + " not found";
		return;
	}
	int sourceLineId = getSourceLineId.GetInt(0);

	Statement getCurrentLineContent = Statement(m_connection, "select SourceLineContentId, Version, Text from SourceLineContents where SourceLineId=(?1)", sourceLineId);
	getCurrentLineContent.Step();

	// Push previous line to historic
	Statement insert(m_connection, "insert into SourceLineContentsHistory(Version, Text, SourceLineContentId) values(?1, ?2, ?3)",
		getCurrentLineContent.GetInt(1),
		getCurrentLineContent.GetString(2),
		getCurrentLineContent.GetInt(0));
	insert.Execute();

	Statement updateCurrentContent(m_connection, "UPDATE SourceLineContents SET Version=(?1), Text=(?2) WHERE SourceLineContentId=(?3)",
		getCurrentLineContent.GetInt(1) + 1,
		m_text,
		getCurrentLineContent.GetInt(0));
	updateCurrentContent.Execute();

}

bool ModifySourceLine::Commit()
{
	try
	{
		Execute(m_connection, "begin");
		InternalModifySourceLine();
		Execute(m_connection, "commit");
	}
	catch (sqliteWrapped::Exception const & e)
	{
		Execute(m_connection, "rollback");
		m_lastErrorMessage = e.Message;
		return false;
	}
	return true;
}

string const & ModifySourceLine::ErrorMessage()
{
	return m_lastErrorMessage;
}