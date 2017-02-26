#pragma once

#include <Windows.h>
#include <string>

namespace lineMangler
{
	std::wstring GetExecutablePath()
	{
		wchar_t buffer[MAX_PATH];
		GetModuleFileName(NULL, buffer, MAX_PATH);
		return std::wstring(buffer);
	}
}