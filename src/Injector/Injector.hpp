#pragma once

///====================================================================================================================================
/// File:				Injector.hpp
/// Author:				MyLibh
/// Created:			15.07.2019
///
/// Last modified by:	MyLibh
/// Last modified on:	15.07.2019
///====================================================================================================================================
/// Copyright(c) MyLibh. All rights reserved.
///====================================================================================================================================

#ifndef __INJECTOR_HPP_INCLUDED__
#define __INJECTOR_HPP_INCLUDED__

/// [url] https://docs.microsoft.com/ru-ru/windows/win32/apiindex/windows-api-list [/url]
#include <Windows.h> 
/// [url] http://en.cppreference.com/w/cpp/header/string [/url]
#include <string>

#include "StringMacro.hpp"

///====================================================================================================================================
/// <summary>   A Win32/64 .dll injector. </summary>
///
/// <remarks>	MyLibh, 15.07.2019. </remarks>
///====================================================================================================================================

class Injector
{
public:
	///====================================================================================================================================
	/// <summary>   Injects. </summary>
	///
	/// <remarks>	MyLibh, 15.07.2019. </remarks>
	///
	/// <param name="pid">  The Process ID. </param>
	/// <param name="path"> Path to the file. </param>
	///====================================================================================================================================
	static void inject(const DWORD pid, const std::wstring& path);
	
	///====================================================================================================================================
	/// <summary>   Injects. </summary>
	///
	/// <remarks>   MyLibh, 15.07.2019. </remarks>
	///
	/// <param name="pid">  The Process ID. </param>
	/// <param name="path"> Path to the file. </param>
	///====================================================================================================================================
	static void inject(const DWORD pid, const std::string& path);

	///====================================================================================================================================
	/// <summary>   Ejects. </summary>
	///
	/// <remarks>   MyLibh, 15.07.2019. </remarks>
	///
	/// <param name="pid">  The Process ID. </param>
	/// <param name="path"> Path to the file. </param>
	///====================================================================================================================================
	static void eject(const DWORD pid, const std::wstring& path);
	
	///====================================================================================================================================
	/// <summary>   Ejects. </summary>
	///
	/// <remarks>   MyLibh, 15.07.2019. </remarks>
	///
	/// <param name="pid">  The Process ID. </param>
	/// <param name="path"> Path to the file. </param>
	///====================================================================================================================================
	static void eject(const DWORD pid, const std::string& path);
	
	///====================================================================================================================================
	/// <summary>   Gets full path from module name. </summary>
	///
	/// <remarks>   MyLibh, 15.07.2019. </remarks>
	///
	/// <param name="module_name"> Name of the module. </param>
	///
	/// <returns>	Full path to the file. </returns>
	///====================================================================================================================================
	static std::tstring get_module_path(const std::tstring& module_name);

	///====================================================================================================================================
	/// <summary>   Gets Process ID by name. </summary>
	///
	/// <remarks>   MyLibh, 15.07.2019. </remarks>
	///
	/// <param name="process_name"> Name of the process. </param>
	///
	/// <returns>	The Process ID. </returns>
	///====================================================================================================================================
	_NODISCARD
	static DWORD get_pid_by_name(const std::tstring& process_name);

	///====================================================================================================================================
	/// <summary>   Gets Process ID by window name. </summary>
	///
	/// <remarks>   MyLibh, 15.07.2019. </remarks>
	///
	/// <param name="window_name"> Name of the window. </param>
	///
	/// <returns>	The Process ID. </returns>
	///====================================================================================================================================
	_NODISCARD
	static DWORD get_pid_by_window(const std::tstring& window_name);

	///====================================================================================================================================
	/// <summary>   Gives the current process the SeDebugPrivelige. </summary>
	///
	/// <remarks>   MyLibh, 15.07.2019. </remarks>
	///====================================================================================================================================
	static void get_se_debug_privilege();
};

#endif /* __INJECTOR_HPP_INCLUDED__ */