#pragma once

#include <Windows.h>

#include <codecvt>
#include <fstream>
#include <locale>
#include <string>

namespace sillyUtils
{
	bool is_file_exist(const std::wstring & fileName);
	std::wstring GetExecutablePath();
	std::wstring GetExecutableDir();
	std::wstring ConvertToWide(const std::string & text);
	std::string ConvertToStandard(const std::wstring & text);
}