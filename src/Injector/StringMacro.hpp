#pragma once

///====================================================================================================================================
/// File:				StringMacro.hpp
/// Author:				MyLibh
/// Created:			15.07.2019
///
/// Last modified by:	MyLibh
/// Last modified on:	15.07.2019
///====================================================================================================================================
/// Copyright(c) MyLibh. All rights reserved.
///====================================================================================================================================

///====================================================================================================================================
/// <summary>	Wrapper alike TCHAR for STL. </summary>
///
/// <remarks>	MyLibh, 15.07.2019. </remarks>
///====================================================================================================================================

#ifdef _UNICODE
	#define tstring			wstring
	#define tfstream		wfstream
	#define tifstream		wifstream
	#define tofstream		wofstream
	#define tstringstream	wstringstream
	#define tcout			wcout
	#define tcin			wcin
	#define tistream		wistream
	#define tostream		wostream
#else
	#define tstring			string
	#define tfstream		fstream
	#define tifstream		ifstream
	#define tofstream		ofstream
	#define tstringstream	stringstream
	#define tcout			cout
	#define tcin			cin
	#define tistream		istream
	#define tostream		ostream
#endif /* _UNICODE */
