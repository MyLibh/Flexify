#include "SEH.hpp"
#include "..\\StringMacro.hpp"
#include "..\\AutoCleanup.hpp"

/// [url] https://docs.microsoft.com/ru-ru/windows/win32/apiindex/windows-api-list [/url]
#include <Windows.h>
/// [url] https://docs.microsoft.com/en-us/windows/win32/debug/debug-help-library [/url]
#include <DbgHelp.h>
/// [url] http://en.cppreference.com/w/cpp/header/vector [/url]
#include <vector>
/// [url] http://en.cppreference.com/w/cpp/header/string [/url]
#include <string>

long __stdcall generic_unhandled_exception(EXCEPTION_POINTERS* exception)
{
	SYSTEMTIME system_time;
	GetLocalTime(&system_time);

	std::vector<TCHAR> date(10);
	GetDateFormat(LOCALE_USER_DEFAULT, 0, &system_time, TEXT("yyyyMMdd"), &date[0], 10);

	std::vector<TCHAR> time(10);
	GetTimeFormat(LOCALE_USER_DEFAULT, TIME_FORCE24HOURFORMAT, &system_time, TEXT("hhmmss"), &time[0], 10);

	std::tstring path(TEXT("crash-"));
	path.append(&date[0]).append(&time[0]).append(TEXT(".txt"));

	AutoCloseHandle hFile = CreateFile(path.c_str(), GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

	MINIDUMP_EXCEPTION_INFORMATION minidump{};
	minidump.ThreadId          = GetCurrentThreadId();
	minidump.ExceptionPointers = exception;
	minidump.ClientPointers    = TRUE;

	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, (MINIDUMP_TYPE)(MiniDumpWithFullMemory | MiniDumpWithHandleData), &minidump, NULL, NULL);

	return EXCEPTION_EXECUTE_HANDLER;
}
