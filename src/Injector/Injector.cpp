// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Injector.hpp"
#include "AutoCleanup.hpp"
#include "StringUtil.hpp"

/// [url] https://docs.microsoft.com/en-us/windows/win32/api/tlhelp32/ [/url]
#include <TlHelp32.h>
/// [url] https://docs.microsoft.com/ru-ru/cpp/text/generic-text-mappings-in-tchar-h?view=vs-2019 [/url]
#include <tchar.h>
/// [url] https://en.cppreference.com/w/cpp/header/vector [/url]
#include <vector>
/// [url] https://en.cppreference.com/w/cpp/header/stdexcept [/url]
#include <stdexcept> 

void Injector::inject(const DWORD pid, const std::wstring& path)
{
	AutoCloseHandle hProcess(OpenProcess(
		PROCESS_QUERY_INFORMATION | // Required by Alpha
		PROCESS_CREATE_THREAD |		// For CreateRemoteThread
		PROCESS_VM_OPERATION |		// For VirtualAllocEx/VirtualFreeEx
		PROCESS_VM_WRITE,           // For WriteProcessMemory
		FALSE, 
		pid));
	if (!hProcess)
		throw std::runtime_error("[!] Could not get handle to process.");

	size_t path_size = (path.length() + 1) * sizeof(wchar_t);

	AutoReleaseRegionEx alloc(VirtualAllocEx(hProcess, nullptr, path_size, MEM_COMMIT, PAGE_READWRITE), hProcess);
	if (!alloc)
		throw std::runtime_error("[!] Could not allocate memory in remote process.");

	if (!WriteProcessMemory(hProcess, alloc, path.c_str(), path_size, nullptr))
		throw std::runtime_error("[!] Could not write to memory in remote process.");;

	HMODULE hKernel32 = GetModuleHandle(TEXT("kernel32"));
	if (!hKernel32) //-V2507
		throw std::runtime_error("[!] Could not get handle to 'kernel32.dll'.");

	PTHREAD_START_ROUTINE load_library = reinterpret_cast<PTHREAD_START_ROUTINE>(GetProcAddress(hKernel32, "LoadLibraryW"));
	if (!load_library) //-V2507
		throw std::runtime_error("[!] Could not get pointer to LoadLibraryW.");;

	AutoCloseHandle hThread(CreateRemoteThread(hProcess, nullptr, 0LLU, load_library, alloc, 0LLU, nullptr));
	if (!hThread)
		throw std::runtime_error("[!] Could not create thread in remote process.");

	WaitForSingleObject(hThread, INFINITE); //-V2523

	DWORD exit_code{};
	if (!GetExitCodeThread(hThread, &exit_code)) //-V2507
		throw std::runtime_error("[!] Could not get thread exit code.");

	if (!exit_code) //-V2507
		throw std::runtime_error("[!] Call to LoadLibraryW in remote process failed.");
}

void Injector::inject(const DWORD pid, const std::string& path)
{
	std::wstring unicode_path(path.begin(), path.end());

	Injector::inject(pid, unicode_path);
}

void Injector::eject(const DWORD pid, const std::wstring& path)
{
	AutoCloseHandle hSnapshot(CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid));
	if (hSnapshot == INVALID_HANDLE_VALUE) //-V2507
		throw std::runtime_error("[!] Could not get module snapshot for remote process.");;

	MODULEENTRY32W me{ sizeof(me) };
	bool found{};
	BOOL module = Module32FirstW(hSnapshot, &me);
	for (; module; module = Module32NextW(hSnapshot, &me))
	{
		std::wstring module_name(me.szModule);
		std::wstring exe_path(me.szExePath);

		found = (module_name == path || exe_path == path);
		if (found)
			break;
	}

	if (!found) 
		throw std::runtime_error("[!] Could not find module in remote process.");;

	AutoCloseHandle hProcess(OpenProcess(
		PROCESS_QUERY_INFORMATION | // Required by Alpha
		PROCESS_CREATE_THREAD |     // For CreateRemoteThread
		PROCESS_VM_OPERATION,		// For CreateRemoteThread
		FALSE, 
		pid));
	if (!hProcess) //-V2507
		throw std::runtime_error("[!] Could not get handle to process.");

	HMODULE hKernel32 = GetModuleHandle(TEXT("kernel32"));
	if (!hKernel32)
		throw std::runtime_error("[!] Could not get handle to 'kernel32.dll'.");

	PTHREAD_START_ROUTINE free_library = reinterpret_cast<PTHREAD_START_ROUTINE>(GetProcAddress(hKernel32, "FreeLibrary"));
	if (!free_library)
		throw std::runtime_error("[!] Could not get pointer to FreeLibrary.");

	AutoCloseHandle hThread(CreateRemoteThread(hProcess, nullptr, 0LLU, free_library, me.modBaseAddr, 0LLU, nullptr));
	if (!hThread)
		throw std::runtime_error("Could not create thread in remote process.");

	WaitForSingleObject(hThread, INFINITE); //-V2523

	DWORD exit_code{};
	if (!GetExitCodeThread(hThread, &exit_code))
		throw std::runtime_error("Could not get thread exit code.");

	if (!exit_code)
		throw std::runtime_error("Call to FreeLibrary in remote process failed.");
}

void Injector::eject(const DWORD pid, const std::string& path)
{
	std::wstring unicode_path(path.begin(), path.end());

	Injector::eject(pid, unicode_path);
}

std::tstring Injector::get_module_path(const std::tstring& module_name)
{
	HMODULE self = GetModuleHandle(nullptr);

	std::vector<TCHAR> loader_path(MAX_PATH);
	if (!GetModuleFileName(self, &loader_path[0], static_cast<DWORD>(loader_path.size())) || GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		throw std::runtime_error("[!] Could not get path to loader.");

	std::tstring module_path(&loader_path[0]);
	module_path = module_path.substr(0LLU, module_path.rfind(_T('\\')) + 1LLU);
	module_path.append(module_name);

	if (GetFileAttributes(module_path.c_str()) == INVALID_FILE_ATTRIBUTES)
	{
		std::string new_module_path(module_path.begin(), module_path.end());

		throw std::runtime_error("[!] Could not find module. Path: '" + new_module_path + "'.");
	}

	return module_path;
}

DWORD Injector::get_pid_by_name(const std::tstring& process_name)
{
	AutoCloseHandle hSnapshot(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0LLU));
	if (hSnapshot == INVALID_HANDLE_VALUE)
		throw std::runtime_error("[!] Could not get process snapshot.");

	PROCESSENTRY32 pe{ sizeof(pe) };
	bool found = false;
	BOOL MoreMods = Process32First(hSnapshot, &pe);
	for (; MoreMods; MoreMods = Process32Next(hSnapshot, &pe))
	{
		std::tstring current_process(pe.szExeFile);

		current_process = to_lower(current_process);

		found = (current_process == process_name);
		if (found)
			break;
	}

	if (!found)
		throw std::runtime_error("[!] Could not find process.");

	return pe.th32ProcessID;
}

DWORD Injector::get_pid_by_window(const std::tstring& window_name)
{
	auto hWnd = FindWindow(NULL, window_name.c_str());
	if (!hWnd)
		throw std::runtime_error("[!] Could not find window.");

	DWORD pid{};
	GetWindowThreadProcessId(hWnd, &pid);
	if (!pid)
		throw std::runtime_error("[!] Could not get process id from window.");

	return pid;
}

void Injector::get_se_debug_privilege()
{
	HANDLE tmp_token{};
	const auto ret_val = OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &tmp_token);
	if (!ret_val)
		throw std::runtime_error("[!] Could not open process token.");

	AutoCloseHandle token(std::move(tmp_token));

	LUID luid{};
	if (!LookupPrivilegeValue(nullptr, SE_DEBUG_NAME, &luid))
		throw std::runtime_error("[!] Could not look up privilege value for SeDebugName.");
	
	if (luid.LowPart == NULL && luid.HighPart == NULL)
		throw std::runtime_error("[!] Could not get LUID for SeDebugName.");

	TOKEN_PRIVILEGES privileges{};
	privileges.PrivilegeCount           = 1;
	privileges.Privileges[0].Luid       = luid;
	privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (!AdjustTokenPrivileges(token, FALSE, &privileges, sizeof(privileges), nullptr, nullptr))
		throw std::runtime_error("[!] Could not adjust token privileges.");
}
