#pragma once

///====================================================================================================================================
/// File:				StringMacro.hpp
/// Author:				MyLibh
/// Created:			22.07.2019
///
/// Last modified by:	MyLibh
/// Last modified on:	22.07.2019
///====================================================================================================================================
/// Copyright(c) MyLibh. All rights reserved.
///====================================================================================================================================

#ifndef __BACKGROUNDCHANGER_HPP_INCLUDED__
#define __BACKGROUNDCHANGER_HPP_INCLUDED__

/// [url] https://en.cppreference.com/w/cpp/filesystem [/url]
#include <filesystem>
// #include <thread>

namespace csgo
{
	///====================================================================================================================================
	/// <summary>   CSGO background changer. </summary>
	///
	/// <remarks>	MyLibh, 22.07.2019. </remarks>
	///====================================================================================================================================

	class BackgroundChanger
	{
	private:
		inline static const std::filesystem::path PATH_TO_BCKG   = L"csgo\\panorama\\videos";
		inline static const std::filesystem::path FILE_TO_CHANGE = L"sirocco.webm";
		inline static const std::filesystem::path FOLDER_NAME    = L"bckg";
	
	private:
		// static std::thread on_daytime_updater;

	private:
		///====================================================================================================================================
		/// <summary>   Backups the file. </summary>
		///
		/// <remarks>	MyLibh, 22.07.2019. </remarks>
		///====================================================================================================================================
		static void backup();

	public:
		///====================================================================================================================================
		/// <summary>   Adds file to data folder. </summary>
		///
		/// <remarks>	MyLibh, 22.07.2019. </remarks>
		///
		/// <param name="bckg"> Path to background. </param>
		///
		/// <returns>	True with success and false otherwise. </returns>
		///====================================================================================================================================
		static bool add(std::filesystem::path bckg);

		///====================================================================================================================================
		/// <summary>   Changes the file. </summary>
		///
		/// <remarks>	MyLibh, 22.07.2019. </remarks>
		///
		/// <param name="new_bckg"> Path to the new background. </param>
		///
		/// <returns>	True with success and false otherwise. </returns>
		///====================================================================================================================================
		static bool change(std::filesystem::path new_bckg);

		///====================================================================================================================================
		/// <summary>   Restores the file. </summary>
		///
		/// <remarks>	MyLibh, 22.07.2019. </remarks>
		///
		/// <returns>	True with success and false otherwise. </returns>
		///====================================================================================================================================
		static bool restore();

		///====================================================================================================================================
		/// <summary>   Gets filepaths from data folder. </summary>
		///
		/// <remarks>	MyLibh, 22.07.2019. </remarks>
		///
		/// <returns>	Array of paths. </returns>
		///====================================================================================================================================
		_NODISCARD
		static std::vector<std::filesystem::path> get_custom_bckg_paths();
	};
} // namespace csgo

#endif /* __BACKGROUNDCHANGER_HPP_INCLUDED__ */