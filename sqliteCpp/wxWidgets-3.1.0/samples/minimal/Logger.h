#pragma once

#include <string>

__interface ILogger
{
	void AddToLog(const std::wstring &message);
	void AddToLog(const std::string &message);
};
