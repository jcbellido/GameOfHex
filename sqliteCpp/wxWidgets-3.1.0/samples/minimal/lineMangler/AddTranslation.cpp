#include "AddTranslation.h"

using namespace lineMangler;
using namespace sqliteWrapped;

bool AddTranslationToSourceLine::Queue()
{
	try
	{
		InternalAddTranslationToSourceLine();
	}
	catch (sqliteWrapped::Exception const & e)
	{
		m_lastErrorMessage = e.Message;
		return false;
	}
	return true;
}

void AddTranslationToSourceLine::InternalAddTranslationToSourceLine()
{
	Statement getSourceLineId = Statement(m_connection, "select SourceLineId from SourceLines where StringId=(?1)", m_stringID);
	if (!getSourceLineId.Step())
	{
		m_lastErrorMessage = "String ID " + m_stringID + " not found";
		return;
	}
	int sourceLineId = getSourceLineId.GetInt(0);

	Statement getSourceLineContentId = Statement(m_connection, "select SourceLineContentId from SourceLineContents where SourceLineId=(?1)", sourceLineId);
	if (!getSourceLineId.Step())
	{
		m_lastErrorMessage = "Source Line Id not found in SourceLineContentId " + sourceLineId;
		return;
	}
	int sourceLineContentId = getSourceLineId.GetInt(0);

	// #jcb What happens if we already have the version? 
	// #jcb SIMPLIFY: we're going to assume that all this is working reasonably well

	Statement getCurrentTranslation = Statement(m_connection, "select TranslationContentId, Version, Text from TranslationContents where sourceLineContentId=(?1) AND LanguageId=(?2)", sourceLineContentId, m_languageCode);
	if (getCurrentTranslation.Step())
	{
			// Push previous line to historic
			Statement insert(m_connection, "insert into TranslationContentsHistory(Version, Text, TranslationContentId) values(?1, ?2, ?3)",
				getCurrentTranslation.GetInt(1),
				getCurrentTranslation.GetString(2),
				getCurrentTranslation.GetInt(0));
			insert.Execute();

			Statement updateCurrentContent(m_connection, "UPDATE TranslationContents SET Version=(?1), Text=(?2) WHERE SourceLineContentId=(?3)",
				m_version,
				m_text,
				sourceLineContentId);
			updateCurrentContent.Execute();
	}
	else
	{
		Statement insert(m_connection, "insert into TranslationContents(Version, Text, SourceLineContentId, LanguageId) values(?1, ?2, ?3, ?4)",
			m_version,
			m_text,
			sourceLineContentId,
			m_languageCode);
		insert.Execute();
	}
}

bool AddTranslationToSourceLine::Commit()
{
	try
	{
		Execute(m_connection, "begin");
		InternalAddTranslationToSourceLine();
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

string const & AddTranslationToSourceLine::ErrorMessage()
{
	return m_lastErrorMessage;
}
