#pragma once

#include <string>

#include "../sqliteWrapped/sqliteWrapped.h"

using namespace std;

namespace lineMangler
{
	__interface ILineManglerCommand
	{
		bool Commit();
		string const & ErrorMessage();
	};
}
