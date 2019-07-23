#pragma once

///====================================================================================================================================
/// File:				MaploadBckgChanger.hpp
/// Author:				MyLibh
/// Created:			24.07.2019
///
/// Last modified by:	MyLibh
/// Last modified on:	24.07.2019
///====================================================================================================================================
/// Copyright(c) MyLibh. All rights reserved.
///====================================================================================================================================

#ifndef __MAPLOADCHANGER_HPP_INCLUDED__
#define __MAPLOADCHANGER_HPP_INCLUDED__

#include "StringMacro.hpp"

/// [url] https://en.cppreference.com/w/cpp/header/string [/url]
#include <string>
/// [url] https://en.cppreference.com/w/cpp/filesystem [/url]
#include <filesystem>	

namespace csgo
{
	///====================================================================================================================================
	/// <summary>   CSGO mapload background changer. </summary>
	///
	/// <remarks>	MyLibh, 24.07.2019. </remarks>
	///====================================================================================================================================

	class MaploadBckgChanger
	{
	private:
		inline static const std::filesystem::path PATH_TO_BCKG = L"csgo\\maps";
		inline static const std::filesystem::path FOLDER_NAME  = L"mapload_bckg";

	private:
		///====================================================================================================================================
		/// <summary>   Backups the file. </summary>
		///	
		/// <remarks>	MyLibh, 24.07.2019. </remarks>
		///
		/// <param name="filename"> Filename to backup. </param>
		///====================================================================================================================================
		static void backup(std::filesystem::path filename);

	public:
		enum maps : int
		{
			ar_baggage,
			ar_dizzy,
			ar_monastery,
			ar_shoots,
			cs_agency,
			cs_assault,
			cs_italy,
			cs_militia,
			cs_office,
			cs_workout,
			de_bank,
			de_cache,
			de_canals,
			de_cbble,
			de_dust2,
			de_inferno,
			de_lake,
			de_mirage,
			de_nuke,
			de_overpass,
			de_safehouse,
			de_shortnuke,
			de_stmarc,
			de_sugarcane,
			de_train,
			de_vertigo,
			de_zoo,

			NUM
		};

		///====================================================================================================================================
		/// <summary>   Adds file to data folder. </summary>
		///
		/// <remarks>	MyLibh, 24.07.2019. </remarks>
		///
		/// <param name="bckg"> Path to background. </param>
		///
		/// <returns>	True with success and false otherwise. </returns>
		///====================================================================================================================================
		static bool add(std::filesystem::path bckg);

		///====================================================================================================================================
		/// <summary>   Changes the file. </summary>
		///
		/// <remarks>	MyLibh, 24.07.2019. </remarks>
		/// 
		/// <param name="map">      Map id. </param>
		/// <param name="new_bckg"> Path to the new background. </param>
		///
		/// <returns>	True with success and false otherwise. </returns>
		///====================================================================================================================================
		static bool change(MaploadBckgChanger::maps map, std::filesystem::path new_bckg);

		///====================================================================================================================================
		/// <summary>   Restores the file. </summary>
		///
		/// <remarks>	MyLibh, 24.07.2019. </remarks>
		///
		/// <param name="map">	Map id. </param>
		///
		/// <returns>	True with success and false otherwise. </returns>
		///====================================================================================================================================
		static bool restore(MaploadBckgChanger::maps map);

		///====================================================================================================================================
		/// <summary>   Translates map id to string. </summary>
		///
		/// <remarks>	MyLibh, 24.07.2019. </remarks>
		///
		/// <param name="map">	Map id. </param>
		///
		/// <returns>	Map name. </returns>
		///====================================================================================================================================
		_NODISCARD
		static std::tstring get_map_name_by_id(maps map);

		///====================================================================================================================================
		/// <summary>   Gets filepaths from data folder. </summary>
		///
		/// <remarks>	MyLibh, 24.07.2019. </remarks>
		///
		/// <returns>	Array of paths. </returns>
		///====================================================================================================================================
		_NODISCARD
		static std::vector<std::filesystem::path> get_custom_mapload_bckg_paths();
	};
} // namespace csgo

#endif /* __MAPLOADCHANGER_HPP_INCLUDED__ */