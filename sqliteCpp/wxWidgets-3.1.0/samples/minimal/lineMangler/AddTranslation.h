#pragma once

#include <string>
#include "../sqliteWrapped/sqliteWrapped.h"
#include "ILineManglerCommand.h"

using namespace std;

namespace lineMangler
{
	class AddTranslationToSourceLine : public ILineManglerCommand
	{
	private:
		sqliteWrapped::Connection & m_connection;
		string m_stringID;
		string m_text;
		string m_lastErrorMessage;
		int m_version; 
		int m_languageCode;

	private:
		void InternalAddTranslationToSourceLine();

	public:
		AddTranslationToSourceLine(sqliteWrapped::Connection & connection, 
			string stringID, // to sourceLineId --> to sourceLineContentID
			int languageId, 
			int versionImported,	// Since all the translations correspond to a Version of the sourceLineContent
			string translatedText) :
			m_connection(connection),
			m_stringID(stringID),
			m_text(translatedText),
			m_lastErrorMessage("")
		{ }

		bool Commit();
		bool Queue();
		string const & ErrorMessage();
	};
}
