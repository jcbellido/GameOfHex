#include "LineMangler.h"

#include <fstream>
#include <iostream>
#include <iomanip>

using namespace std;

void lineMangler::LineLoader::LoadFiles()
{
	m_filenames = { "quijote_source_ep1_en.txt", "quijote_source_ep1_ar.txt", "quijote_source_ep1_es.txt", "quijote_source_ep1_fr.txt", "quijote_source_ep1_zh.txt" };
	for (auto & thing : m_filenames)
	{
		cout << thing << endl;
		list<string> readLines; 
	
		ifstream myfile(thing);
		if (myfile.is_open())
		{
			while (!myfile.eof())
			{
				std::string line;
				myfile >> line;
				readLines.push_back(line);
			}
			myfile.close();
		}
		else
		{
			cout << "Was trying to read " << thing << " but something mega fucky happened!" << endl;
		}
		m_languageLineMap.insert(pair<string, list<string>>(thing, readLines));
	}
}

const list<string>& lineMangler::LineLoader::GetLanguageLines(int language)
{
	switch (language)
	{
		case LanguageCode::Arabic:
			return m_languageLineMap["quijote_source_ep1_ar.txt"];
		case LanguageCode::English:
			return m_languageLineMap["quijote_source_ep1_en.txt"];
		case LanguageCode::French:
			return m_languageLineMap["quijote_source_ep1_fr.txt"];
		case LanguageCode::Spanish:
			return m_languageLineMap["quijote_source_ep1_es.txt"];
		case LanguageCode::TraditionalChinese:
			return m_languageLineMap["quijote_source_ep1_zh.txt"];
		default:
			return m_languageLineMap["quijote_source_ep1_en.txt"];
	}
}
