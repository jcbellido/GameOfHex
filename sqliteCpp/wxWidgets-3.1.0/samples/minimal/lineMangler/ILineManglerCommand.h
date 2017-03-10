#pragma once

#include <string>

#include "../sqliteWrapped/sqliteWrapped.h"

using namespace std;

namespace lineMangler
{
	__interface ILineManglerCommand
	{
		bool Commit();
		bool Queue();
		string const & ErrorMessage();
	};
}
