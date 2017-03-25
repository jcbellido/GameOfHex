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

std::wstring sillyUtils::ConvertToWide(const std::string & text)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(text);
}

std::string sillyUtils::ConvertToStandard(const std::wstring & text)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.to_bytes(text);
}

std::wstring sillyUtils::GetExecutableDir()
{
	auto executablePath = GetExecutablePath();
	size_t found;
	found = executablePath.find_last_of(L"/\\");
	return std::wstring(executablePath.substr(0, found));
}


