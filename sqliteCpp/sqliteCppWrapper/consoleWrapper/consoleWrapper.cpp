#include "sqlite3\sqlite3.h"
#include <stdio.h>

int main()
{
	sqlite3 * connection = nullptr;

	int result = sqlite3_open(":memory:", &connection);

	if (SQLITE_OK != result)
	{
		printf("%s/n", sqlite3_errmsg(connection));
		sqlite3_close(connection);
		return result;
	} 
	else
	{ 
		sqlite3_close(connection);
	}
	
    return 0;
}

