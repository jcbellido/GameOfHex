#pragma once

#include <string>
#include "../sqliteWrapped/sqliteWrapped.h"
#include "ILineManglerCommand.h"

using namespace std;

namespace lineMangler
{
	class AssignStringToLineSet : public ILineManglerCommand
	{
	private:
		sqliteWrapped::Connection & m_connection;
		int m_sourceLineID;
		int m_lineSetID;
		string m_lastErrorMessage;

	private:
		void InternalAssignStringToLineSet();

	public:
		AssignStringToLineSet(sqliteWrapped::Connection & connection,
			int sourceLineID,
			int lineSetID) :
			m_connection(connection),
			m_sourceLineID(sourceLineID),
			m_lineSetID(lineSetID),
			m_lastErrorMessage("")
		{ }

		bool Commit();
		bool Queue();
		string const & ErrorMessage();
	};
}
