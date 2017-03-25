#pragma once

#include <string>
#include <map>

#include "../sqliteWrapped/sqliteWrapped.h"

using namespace std;

namespace lineMangler
{
	//enum LanguageCode
	//{
	//	Arabic,
	//	English,
	//	Spanish,
	//	French,
	//	TraditionalChinese,
	//	EndOfLanguageCode
	//};

	class LanguageMap
	{
	private:
		sqliteWrapped::Connection & m_connection;

	public: 
		LanguageMap(sqliteWrapped::Connection & connection):
			m_connection(connection)
		{ }
	};
}