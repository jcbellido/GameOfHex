#pragma once

#include <Windows.h>

#include <fstream>
#include <string>

namespace sillyUtils
{
	bool is_file_exist(const std::wstring & fileName);
	std::wstring GetExecutablePath();
}