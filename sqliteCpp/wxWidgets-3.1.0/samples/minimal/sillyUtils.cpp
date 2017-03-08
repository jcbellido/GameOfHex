#include "sillyUtils.h"

bool sillyUtils::is_file_exist(const std::wstring & fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}

std::wstring sillyUtils::GetExecutablePath()
{
	wchar_t buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	return std::wstring(buffer);
}
