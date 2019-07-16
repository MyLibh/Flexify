// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/// [url] https://docs.microsoft.com/ru-ru/cpp/text/generic-text-mappings-in-tchar-h?view=vs-2019 [/url]
#include <tchar.h>
/// [url] https://docs.microsoft.com/en-us/windows/win32/api/shellapi [/url]
#include <Shlwapi.h>
/// [url] http://en.cppreference.com/w/cpp/header/iostream [/url]
#include <iostream>
/// [url] http://en.cppreference.com/w/cpp/header/vector [/url]
#include <vector>

#include "Injector.hpp"
#include "StringUtil.hpp"
#include "SEH/SEH.hpp"
#include "argh.h"

#pragma comment(lib, "shlwapi")

constexpr std::tstring_view VERSION(_T("20190716"));

signed main(int, char** argv)
{
	try
	{
		seh_guard seh;

#ifdef _WIN64
		std::tcout << _T("Injector x64 [Version ") << VERSION << _T("]") << std::endl;
#else
		std::tcout << _T("Injector x86 [Version ") << VERSION << _T("]") << std::endl;
#endif
		std::tcout << _T("Copyright(c) 2019 MyLibh. All rights reserved.") << std::endl << std::endl;

		argh::parser cmd;

		cmd.add_params({
			"n", "process-name",
			"w", "window-name",
			"p", "process-id"
			});

		cmd.parse(argv);

		if (cmd[{ "-h", "--help" }])
		{
			std::tcout << _T("usage: Injector [options] [modules]") << std::endl << std::endl;
			std::tcout << _T("  options:") << std::endl;
			std::tcout << _T("    specify at least one of the following methods:") << std::endl;
			std::tcout << _T("      -n, --process-name        Identify target process by process name") << std::endl;
			std::tcout << _T("      -w, --window-name         Identify target process by window title") << std::endl;
			std::tcout << _T("      -p, --process-id          Identify target process by numeric ID") << std::endl << std::endl;
			std::tcout << _T("    specify at least one of the following actions:") << std::endl;
			std::tcout << _T("      -i, --inject              Inject/load referenced module") << std::endl;
			std::tcout << _T("      -e, --eject               Eject/unload referenced module") << std::endl << std::endl;
			std::tcout << _T("  modules:") << std::endl;
			std::tcout << _T("      myLib.dll [anotherLib.dll] [C:\\dll\\anotherLib2.dll]") << std::endl;
			std::tcout << std::endl;

			return 0;
		}

		if (cmd.pos_args().size() <= 1)
		{
			std::tcout << _T("[!] No module name(s) or path(s) specified!") << std::endl;

			return ERROR_INVALID_PARAMETER;
		}

		if (!cmd[{ "-i", "--inject", "-e", "--eject" }])
		{
			std::tcout << _T("[!] No action specified!") << std::endl;

			return ERROR_INVALID_PARAMETER;
		}

		if (cmd[{ "-i", "--inject" }] && cmd[{ "-e", "--eject" }])
		{
			std::tcout << _T("[!] Only one action at a time allowed!") << std::endl;

			return ERROR_INVALID_PARAMETER;
		}

		if (!(cmd({ "-n", "--process-name" }))&& !cmd({ "-w", "--window-name" }) && !cmd({ "-p", "--process-id" }))
		{
			std::tcout << _T("[!] No process identifier specified!") << std::endl;

			return ERROR_INVALID_PARAMETER;
		}

		DWORD pid{ };
		std::tstring module_path;
		std::string optArg;

		if (cmd({ "-n", "--process-name" }) >> optArg)
			pid = Injector::get_pid_by_name(utf8_to_utf16(to_lower(optArg)));

		if (cmd({ "-w", "--window-name" }) >> optArg)
			pid = Injector::get_pid_by_window(utf8_to_utf16(optArg));

		if (cmd({ "-p", "--process-id" }) >> optArg)
		{
			pid = _tstoi(utf8_to_utf16(optArg).c_str());

			if (!pid)
				throw std::exception("[!] nvalid PID entered!");
		}

		Injector::get_se_debug_privilege();

		std::vector<std::wstring> modules;

		for (auto it = std::next(cmd.pos_args().begin()); it != cmd.pos_args().end(); ++it)
			modules.push_back(utf8_to_utf16(*it));

		if (cmd[{ "-i", "--inject" }])
			for (auto& mod : modules)
			{
				module_path = PathIsRelative(mod.c_str()) ? Injector::get_module_path(mod) : mod;

				Injector::inject(pid, module_path);

				std::tcout << _T("[+] Successfully injected module.") << std::endl;
			}

		if (cmd[{ "-e", "--eject" }])
		{
			for (auto& mod : modules)
			{
				module_path = PathIsRelative(mod.c_str()) ? Injector::get_module_path(mod) : mod;

				Injector::eject(pid, module_path);

				std::tcout << _T("[+] Successfully ejected module.") << std::endl;
			}
		}
	}
	catch (const std::exception& e)
	{
		std::string tmp_error(e.what());
		std::tstring error(tmp_error.begin(), tmp_error.end());
		std::tcout << _T("[!] Error:") << std::endl << error << std::endl;
	}
	catch (const seh_exception& e)
	{
		std::tcout << _T("[!] SEH Error:") << std::endl << e.get_code() << std::endl;
	}
	catch (...)
	{
		std::tcout << _T("[!] Unknown error!") << std::endl;
	}

	return 0;
}