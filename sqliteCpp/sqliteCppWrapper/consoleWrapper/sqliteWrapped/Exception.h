#pragma once

#include <string>

#include "CommonMacros.h"
#include "sqlite3.h"

struct Exception
{
	int Result = 0;
	std::string Message;

	explicit Exception(sqlite3 * const connection): 
		Result(sqlite3_extended_errcode(connection)),
		Message(sqlite3_errmsg(connection))
	{ }
};

