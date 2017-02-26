#include <iostream>
#include <string>
#include <Windows.h>

#include "sqliteWrapped\sqliteWrapped.h"
#include "testing/LineMangler.h"

using namespace sqliteWrapped;
using namespace lineMangler;


std::wstring GetExecutablePath()
{
	wchar_t buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	return std::wstring(buffer);
}

//connection.Profile([](void *, char const * const statement, unsigned long long const time)
//{
//	unsigned long long const ms = time / 1000000;
//	if (ms > 10)
//	{
//		std::cout << "Profiler " << ms << " for statement " << statement << std::endl;
//	}
//});


int main()
{
	try
	{
		// Connection connection = Connection::Memory();

		auto executablePath = GetExecutablePath();
		executablePath += L".db";
		Connection connection = Connection(executablePath.c_str());

		// Execute(connection, "create table Things (Content)");

		Statement insert(connection, "insert into Things (Content) values (?1)");

		// Execute(connection, "begin");

		for (int i = 0; i != 1000; ++i)
		{
			insert.Reset(i);
			insert.Execute();
		}

		// Execute(connection, "commit");

		Statement count(connection, "select count(*) from Things");
		count.Step();
		cout << "Count: " << count.GetInt() << endl;

		//for (int i = LanguageCode::Arabic; i != LanguageCode::EndOfLanguageCode; i++)
		//{
		//	auto lines = loader.GetLanguageLines(i);
		//}
	} 
	catch (Exception const & e)
	{
		std::cout << e.Message << "(" << e.Result << ")" << std::endl;
 	}
}

