#pragma once

///====================================================================================================================================
/// File:				StringUtil.hpp
/// Author:				MyLibh
/// Created:			16.07.2019
///
/// Last modified by:	MyLibh
/// Last modified on:	20.07.2019
///====================================================================================================================================
/// Copyright(c) MyLibh. All rights reserved.
///====================================================================================================================================

#ifndef __STRINGUTIL_HPP_INCLUDED__
#define __STRINGUTIL_HPP_INCLUDED__

/// [url] https://docs.microsoft.com/ru-ru/windows/win32/apiindex/windows-api-list [/url]
#include <Windows.h>
/// [url] http://en.cppreference.com/w/cpp/header/string [/url]
#include <string>
/// [url] https://en.cppreference.com/w/cpp/locale/locale [/url]
#include <locale>
/// [url] https://en.cppreference.com/w/cpp/header/algorithm [/url]
#include <algorithm>
/// [url] https://ru.cppreference.com/w/cpp/header/iterator [/url]
#include <iterator>

///====================================================================================================================================
/// <summary>   Converts given string to lower characters. </summary>
///
/// <remarks>	MyLibh, 16.07.2019. </remarks>
///
/// <param name="in"> The String to process. </param>
///====================================================================================================================================
template <typename T>
_NODISCARD
T to_lower(const T& in)
{
	T ans;
	std::transform(in.begin(), in.end(), std::back_inserter(ans), [](typename T::value_type ch) { return std::tolower(ch, std::locale()); });

	return ans;
}

///====================================================================================================================================
/// <summary>   Converts given UTF-8 string to w. </summary>
///
/// <remarks>	MyLibh, 16.07.2019. </remarks>
///
/// <param name="utf8"> String to convert. </param>
///
/// <returns> Converted String. </returns>
///====================================================================================================================================
inline std::wstring utf8_to_utf16(const std::string& utf8) 
{
	std::wstring utf16;

	auto len = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, NULL, 0);
	if (len > 1)
	{
		utf16.resize(static_cast<size_t>(len));

		MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, utf16.data(), len);
	}

	return utf16;
}

#endif /* __STRINGUTIL_HPP_INCLUDED__ */