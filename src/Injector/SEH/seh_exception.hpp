#pragma once

///====================================================================================================================================
/// File:				seh_exception.hpp
/// Author:				MyLibh
/// Created:			16.07.2019
///
/// Last modified by:	MyLibh
/// Last modified on:	16.07.2019
///====================================================================================================================================
/// Copyright(c) MyLibh. All rights reserved.
///====================================================================================================================================

#ifndef __SEH_EXCEPTION_HPP_INCLUDED__
#define __SEH_EXCEPTION_HPP_INCLUDED__

/// [url] https://docs.microsoft.com/ru-ru/windows/win32/apiindex/windows-api-list [/url]
#include <Windows.h>

///====================================================================================================================================
/// <summary>   SEH proxy exception. </summary>
///
/// <remarks>	MyLibh, 16.07.2019. </remarks>
///====================================================================================================================================

class seh_exception
{
public:
	///====================================================================================================================================
	/// <summary>   Simple constructor. </summary>
	///
	/// <remarks>	MyLibh, 16.07.2019. </remarks>
	///====================================================================================================================================
	seh_exception(unsigned code, EXCEPTION_POINTERS* exception);

	///====================================================================================================================================
	/// <summary>   Gets exception code. </summary>
	///
	/// <remarks>	MyLibh, 16.07.2019. </remarks>
	///
	/// <returns> Exception code. </returns>
	///====================================================================================================================================
	unsigned int get_code() const;

	///====================================================================================================================================
	/// <summary>   Gets exception data pointer. </summary>
	///
	/// <remarks>	MyLibh, 16.07.2019. </remarks>
	///
	/// <returns> Exception data pointer. </returns>
	///====================================================================================================================================
	EXCEPTION_POINTERS* get_exception_pointers() const;

private:
	unsigned            m_code;
	EXCEPTION_POINTERS* m_exception;
};

///====================================================================================================================================
/// <summary>   SEH to C++ EH proxy function. </summary>
///
/// <remarks>	MyLibh, 16.07.2019. </remarks>
///
/// <param name="code">      Exception code. </param>
/// <param name="exception"> Exception data pointer. </param>
///====================================================================================================================================
void seh_translator(unsigned code, EXCEPTION_POINTERS* exception);

#endif /* __SEH_EXCEPTION_HPP_INCLUDED__ */