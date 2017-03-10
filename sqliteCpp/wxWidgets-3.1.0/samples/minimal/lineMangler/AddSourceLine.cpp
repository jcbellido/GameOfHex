#include "AddSourceLine.h"

using namespace lineMangler;
using namespace sqliteWrapped;

bool AddSourceLine::Commit() 
{
	// Execute(connection, "create table Things (Content)");
	try
	{
		Execute(m_connection, "begin");
		Statement insert(m_connection, "insert into SourceLines(StringId, Platform, Status) values(?1, ?2, ?3)", m_stringID, m_platform, m_status);
		insert.Execute();
		
		// Keeping the FK reference
		long long rowId = m_connection.RowId();
		Statement checkId = Statement(m_connection, "select SourceLineId from SourceLines where rowId=(?1)", static_cast<int>(rowId));
		checkId.Step();
		int lastIdInserted = checkId.GetInt(0);

		Statement insertContents(m_connection, "insert into SourceLineContents(Version, Text, SourceLineId) values(?1, ?2, ?3)", 0, m_text, lastIdInserted);
		insertContents.Execute();
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

