#pragma once

#include <string>
#include "../sqliteWrapped/sqliteWrapped.h"

using namespace std;

namespace lineMangler
{
	// Creates a completely new source line, requires Platform, StringID, and text
	class AddSourceLine
	{
	private:
		sqliteWrapped::Connection & m_connection;
		string m_stringID;
		string m_text;
		int m_platform;
		int m_status;

		string m_lastErrorMessage;

	public: 
		AddSourceLine(sqliteWrapped::Connection & connection, string stringID, string text, int platform, int status) :
			m_connection(connection),
			m_stringID(stringID),
			m_text(text),
			m_platform(platform),
			m_status(status),
			m_lastErrorMessage("")
		{ }
		// The idea here is to always commit to the on memory data
		bool Commit();
		// bool RollBack();

		string const & ErrorMessage();

	private:
	};
}
