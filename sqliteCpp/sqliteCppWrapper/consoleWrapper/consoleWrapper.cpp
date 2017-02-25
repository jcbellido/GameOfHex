#include "sqliteWrapped\sqliteWrapped.h"
#include "sqliteWrapped\sqlite3.h"
#include <iostream>

int main()
{
	std::cout << "*** sqlite Console wrapper" << std::endl << std::endl;
	try
	{
		Connection connection = Connection::Memory();
		Statement statement; 
		statement.Prepare(connection, "select ?1 union all select ?2");
		statement.Bind(1, "primero");
		statement.Bind(2, L"segundo");

		for (Row const & row : statement)
		{
			std::cout << row.GetString(0) << " " << row.GetInt(1) << std::endl;
		}
	} 
	catch (Exception const & e)
	{
		std::cout << e.Message << "(" << e.Result << ")" << std::endl;
 	}
	std::cout << std::endl << "And done :) ***" << std::endl;
}

