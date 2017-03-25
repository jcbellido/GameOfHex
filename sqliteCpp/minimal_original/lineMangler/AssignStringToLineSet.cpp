#include "AssignStringToLineSet.h"

using namespace lineMangler;
using namespace sqliteWrapped;

bool AssignStringToLineSet::Queue()
{
	try
	{
		InternalAssignStringToLineSet();
	}
	catch (sqliteWrapped::Exception const & e)
	{
		m_lastErrorMessage = e.Message;
		return false;
	}
	return true;
}

void AssignStringToLineSet::InternalAssignStringToLineSet()
{
	Statement insert(m_connection, "insert into LineSetContents(LineSetId, SourceLineId) values(?1, ?2)",
		m_lineSetID, m_sourceLineID);
	insert.Execute();
}

bool AssignStringToLineSet::Commit()
{
	try
	{
		Execute(m_connection, "begin");
		InternalAssignStringToLineSet();
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

string const & AssignStringToLineSet::ErrorMessage()
{
	return m_lastErrorMessage;
}
