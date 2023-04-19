#pragma once

#define LOGNAME L"plugin_log.txt"


class eLog {
public:
	static wchar_t path[260];

	static void Initialize();
	static void Message(const char* function, const char* format, ...);
};