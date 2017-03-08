#pragma once

#include <string>

__interface ILogger
{
	void AddToLog(const std::wstring &message);
};
