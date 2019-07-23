#pragma once

///====================================================================================================================================
/// File:				Util.hpp
/// Author:				MyLibh
/// Created:			19.07.2019
///
/// Last modified by:	MyLibh
/// Last modified on:	19.07.2019
///====================================================================================================================================
/// Copyright(c) MyLibh. All rights reserved.
///====================================================================================================================================

#ifndef __UTIL_HPP_INCLUDED__
#define __UTIL_HPP_INCLUDED__

/// [url] https://docs.microsoft.com/ru-ru/cpp/text/generic-text-mappings-in-tchar-h?view=vs-2019 [/url]
#include <tchar.h>
/// [url] https://en.cppreference.com/w/cpp/filesystem [/url]
#include <filesystem>
// [url] https://en.cppreference.com/w/cpp/header/string_view [/url]
#include <string_view>
// [url] https://en.cppreference.com/w/cpp/header/array [/url]
#include <array>

#include "StringMacro.hpp"

constexpr unsigned long BIT29 = 0x2000000;

namespace csgo
{
	namespace constants
	{
		const std::filesystem::path APP_FOLDER_NAME    = "Flexify";
		const std::filesystem::path DATA_FOLDER_NAME   = "data";
		const std::filesystem::path BACKUP_FOLDER_NAME = "backup";
		const std::filesystem::path BIN_FOLDER_NAME    = "bin";
		const std::filesystem::path INI_FILENAME       = "settings.ini";

		extern const std::filesystem::path PATH_TO_DOCUMENTS;

		extern const std::filesystem::path PATH_TO_INI;
		extern const std::filesystem::path PATH_TO_DATA;
		extern const std::filesystem::path PATH_TO_BACKUP;
		extern const std::filesystem::path PATH_TO_BIN;

		extern const std::filesystem::path PATH_TO_CSGO;

		constexpr size_t NUM_OF_MAPS = 27LLu;
		constexpr std::array<std::tstring_view, NUM_OF_MAPS> MAPS =
		{
			_T("ar_baggage"),
			_T("ar_dizzy"),
			_T("ar_monastery"),
			_T("ar_shoots"),
			_T("cs_agency"),
			_T("cs_assault"),
			_T("cs_italy"),
			_T("cs_militia"),
			_T("cs_office"),
			_T("cs_workout"),
			_T("de_bank"),
			_T("de_cache"),
			_T("de_canals"),
			_T("de_cbble"),
			_T("de_dust2"),
			_T("de_inferno"),
			_T("de_lake"),
			_T("de_mirage"),
			_T("de_nuke"),
			_T("de_overpass"),
			_T("de_safehouse"),
			_T("de_shortnuke"),
			_T("de_stmarc"),
			_T("de_sugarcane"),
			_T("de_train"),
			_T("de_vertigo"),
			_T("de_zoo")
		};
	}	

	///====================================================================================================================================
	/// <summary>   Converts .mp4 file to .webm using ffmpeg. </summary>
	///
	/// <remarks>	MyLibh, 19.07.2019. </remarks>
	///
	/// <param name="input"> Path to .mp4 file. </param>
	///====================================================================================================================================
	void mp4_to_webm(std::filesystem::path input);
}

#endif /* __UTIL_HPP_INCLUDED__ */