#pragma once

#include "CommonMacros.h"
#include "sqlite3.h"

namespace sqliteWrapped
{
	/// Dumps a Statement into a CSV file, trying to reflect on the types of the columns
	/// Usage ideas ... A statement is constructed but not stepped
	/// 1. Statement + destination filename is provided for this class
	/// 2. The dump is executed, per every step of the query
	/// 3. Per existing column --> 
	///		a. Determine the type
	///		b. In case of strings --> add quotes
	///		c. Dump the value
	///		d. Add a comma at the end (unless it's the last field, in that case add a new line) 
	class StatementCSVWriter 
	{

	};
}