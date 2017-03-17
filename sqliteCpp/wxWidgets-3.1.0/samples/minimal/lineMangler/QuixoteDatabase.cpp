#include "QuixoteDatabase.h"

#include <chrono>

#include "lineMangler\LineMangler.h"
#include "AddSourceLine.h"
#include "AddTranslation.h"
#include "AssignStringToLineSet.h"


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

	auto linesSpa = loader.GetLanguageLines(lineMangler::LanguageCode::Spanish);
	auto currentSpa = linesSpa.begin();
	
	auto linesFr = loader.GetLanguageLines(lineMangler::LanguageCode::French);
	auto currentFr = linesFr.begin();
	
	auto linesZh = loader.GetLanguageLines(lineMangler::LanguageCode::TraditionalChinese);
	auto currentZh = linesZh.begin();
	
	auto linesAra = loader.GetLanguageLines(lineMangler::LanguageCode::Arabic);
	auto currentAra = linesAra.begin();

	sqliteWrapped::Execute(m_connection, "PRAGMA synchronous=OFF");
	//Statement pragmaMemory (m_connection, "PRAGMA journal_mode=MEMORY");
	//pragmaMemory.Step();

	const int HARD_LIMIT = 4096;
	int currentWrittingNumber = 0;
	sqliteWrapped::Execute(m_connection, "begin transaction");
	int currentLineSet = 1;
	for (int i = 0; i < m_lines; ++i)
	{
		string stringId = GenerateNewStringID();
		lineMangler::AddSourceLine a(m_connection,
			stringId,
			*currentLine,
			1,	// Hardcoded Platform (Common by design) 
			0);	// Hardcoded Status
		a.Queue();

		lineMangler::AssignStringToLineSet assignLineSet(m_connection, a.GetSourceLineID(), currentLineSet);
		assignLineSet.Queue();

		lineMangler::AddTranslationToSourceLine addTranslationSpa(m_connection, stringId, 2, 0, *currentSpa);
		addTranslationSpa.Queue();

		lineMangler::AddTranslationToSourceLine addTranslationFr(m_connection, stringId, 3, 0, *currentFr);
		addTranslationFr.Queue();

		lineMangler::AddTranslationToSourceLine addTranslationZh(m_connection, stringId, 4, 0, *currentZh);
		addTranslationZh.Queue();

		lineMangler::AddTranslationToSourceLine addTranslationAra(m_connection, stringId, 5, 0, *currentAra);
		addTranslationAra.Queue();

		
		// FIXME there is a bug here FOR SURE
		for (int j = 0; j < m_versions; ++j)
		{
			// Now the Versions 
			lineMangler::ModifySourceLine addVersion(m_connection,
				stringId,
				*currentLine);
			addVersion.Queue();

			lineMangler::AddTranslationToSourceLine addTranslationSpaVer (m_connection, stringId, 2, j, *currentSpa);
			addTranslationSpaVer.Queue();

			lineMangler::AddTranslationToSourceLine addTranslationFrVer(m_connection, stringId, 3, j, *currentFr);
			addTranslationFrVer.Queue();
			
			lineMangler::AddTranslationToSourceLine addTranslationZhVer(m_connection, stringId, 4, j, *currentZh);
			addTranslationZhVer.Queue();

			lineMangler::AddTranslationToSourceLine addTranslationAraVer(m_connection, stringId, 5, j, *currentAra);
			addTranslationAraVer.Queue();
		}

		currentLine++;
		currentSpa++;
		currentFr++;
		currentZh++;
		currentAra++;

		if (currentLine == lines.end())
		{
			currentLine = lines.begin();
			currentSpa = linesSpa.begin();
			currentFr = linesFr.begin();
			currentZh = linesZh.begin();
			currentAra = linesAra.begin();
		}
		
		currentLineSet = currentLineSet == 4 ? 1 : currentLineSet + 1;

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