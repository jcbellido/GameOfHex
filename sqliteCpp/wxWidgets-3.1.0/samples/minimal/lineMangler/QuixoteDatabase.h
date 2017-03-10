#pragma once

#include <string>
#include "../sqliteWrapped/sqliteWrapped.h"
#include "ILineManglerCommand.h"

using namespace std;

namespace lineMangler
{
	/// This is a dummy class, used to populate the existing DB with predefined data
	class QuixoteDatabase: public ILineManglerCommand
	{
	private:
		sqliteWrapped::Connection & m_connection;
		string m_lastErrorMessage;

		int m_lines;
		int m_versions;

	private:
		string GenerateNewStringID();

	public:
		QuixoteDatabase(sqliteWrapped::Connection & connection, int lines, int versions) :
			m_connection(connection),
			m_lines(lines),
			m_versions(versions),
			m_lastErrorMessage("")
		{ }

		bool Commit();
		string const & ErrorMessage();
	};
}
