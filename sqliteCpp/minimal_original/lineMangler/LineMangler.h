#pragma once

#include <iostream>
#include <list>
#include <map>
#include <string>

#include "../sqliteWrapped/CommonMacros.h"
#include "../sqliteWrapped/sqliteWrapped.h"

using namespace std;

namespace lineMangler
{
	enum LanguageCode
	{
		Arabic,
		English,
		Spanish,
		French,
		TraditionalChinese,
		EndOfLanguageCode
	};

	class LineLoader
	{
	private:
		map <string, list<string>> m_languageLineMap;
		list<string> m_filenames;
	public:
		void LoadFiles();
		const list<string> & GetLanguageLines(int language);
	};
}