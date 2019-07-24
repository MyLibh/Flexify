// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "MaploadBckgChanger.hpp"
#include "Util.hpp"
#include "StringMacro.hpp"

/// [url] https://docs.microsoft.com/ru-ru/cpp/text/generic-text-mappings-in-tchar-h?view=vs-2019 [/url]
#include <tchar.h>
/// [url] http://en.cppreference.com/w/cpp/header/iostream [/url]
#include <iostream>

void csgo::MaploadBckgChanger::backup(std::filesystem::path path)
{
	auto path_to_backup = csgo::constants::PATH_TO_BACKUP / MaploadBckgChanger::FOLDER_NAME;

	if (!std::filesystem::is_directory(path_to_backup))
	{
		std::filesystem::remove(path_to_backup);
		std::filesystem::create_directories(path_to_backup);
	}

	path_to_backup /= path.filename();

	if (!std::filesystem::exists(path_to_backup))
	{
		if (!std::filesystem::exists(path))
		{
			std::tcout << _T("[~] Cannot backup file(") << path << _T(").") << std::endl;

			return;
		}

		std::filesystem::rename(path, path_to_backup);

		std::tcout << _T("[+] Successfully backuped(") << path << _T(").") << std::endl;
	}
	else
		std::tcout << _T("[-] Backup is not needed(") << path << _T(").") << std::endl;
}

bool csgo::MaploadBckgChanger::add(std::filesystem::path bckg)
{
	if (!std::filesystem::exists(bckg))
		throw std::invalid_argument("[!] Could not find new mapload background.");

	std::filesystem::path path_to_bckg_data = csgo::constants::PATH_TO_DATA / MaploadBckgChanger::FOLDER_NAME;
	if (!std::filesystem::is_directory(path_to_bckg_data))
	{
		std::filesystem::remove(path_to_bckg_data);
		std::filesystem::create_directories(path_to_bckg_data);
	}

	std::filesystem::rename(bckg, path_to_bckg_data / bckg.filename());

	std::tcout << _T("[+] New mapload background successfully added(") << path_to_bckg_data / bckg.filename() << _T(").") << std::endl;

	return true;
}

bool csgo::MaploadBckgChanger::change(MaploadBckgChanger::maps map, std::filesystem::path new_bckg)
{
	if (!std::filesystem::exists(new_bckg))
		throw std::invalid_argument("[!] Could not find new mapload background.");

	auto filename    = MaploadBckgChanger::get_map_name_by_id(map) + _T(".jpg");
	auto path_to_map = csgo::constants::PATH_TO_CSGO / MaploadBckgChanger::PATH_TO_BCKG / filename;

	MaploadBckgChanger::backup(path_to_map);

	std::error_code ec;
	if (!std::filesystem::copy_file(new_bckg, path_to_map, std::filesystem::copy_options::overwrite_existing, ec))
		throw std::runtime_error("[!] Could not copy mapload background. Error code: " + std::to_string(ec.value()) + " (" + ec.message() + ").");

	std::tcout << _T("[+] Mapload background(") << path_to_map.filename() <<  _T(") successfully changed.") << std::endl;

	return true;
}

bool csgo::MaploadBckgChanger::restore(MaploadBckgChanger::maps map)
{
	auto filename = MaploadBckgChanger::get_map_name_by_id(map) + _T(".jpg");

	std::filesystem::path path_to_backup = csgo::constants::PATH_TO_BACKUP / MaploadBckgChanger::FOLDER_NAME / filename;
	if (!std::filesystem::exists(path_to_backup))
	{
		std::tcout << _T("[~] Mapload background not found. Cannot restore(") << path_to_backup << _T(").") << std::endl;

		return false;
	}

	std::filesystem::path prev_bckg = csgo::constants::PATH_TO_CSGO / MaploadBckgChanger::PATH_TO_BCKG / filename;
	if (std::filesystem::exists(prev_bckg))
		std::filesystem::remove(prev_bckg);

	std::filesystem::rename(path_to_backup, prev_bckg);

	std::tcout << _T("[+] Mapload background successfully restored(") << prev_bckg << _T(").") << std::endl;

	return true;
}

_NODISCARD
std::tstring csgo::MaploadBckgChanger::get_map_name_by_id(maps map)
{
	switch (map)
	{
		case maps::ar_baggage:   return _T("ar_baggage");
		case maps::ar_dizzy:     return _T("ar_dizzy");
		case maps::ar_monastery: return _T("ar_monastery");
		case maps::ar_shoots:    return _T("ar_shoots");
		case maps::cs_agency:	 return _T("cs_agency");
		case maps::cs_assault:   return _T("cs_assault");
		case maps::cs_italy:     return _T("cs_italy");
		case maps::cs_militia:   return _T("cs_militia");
		case maps::cs_office:	 return _T("cs_office");
		case maps::cs_workout:	 return _T("cs_workout");
		case maps::de_bank:      return _T("de_bank");
		case maps::de_cache:     return _T("de_cache");
		case maps::de_canals:    return _T("de_canals");
		case maps::de_cbble:     return _T("de_cbble");
		case maps::de_dust2:     return _T("de_dust2");
		case maps::de_inferno:   return _T("de_inferno");
		case maps::de_lake:      return _T("de_lake");
		case maps::de_mirage:    return _T("de_mirage");
		case maps::de_nuke:      return _T("de_nuke");
		case maps::de_overpass:  return _T("de_overpass");
		case maps::de_safehouse: return _T("de_safehouse");
		case maps::de_shortnuke: return _T("de_shortnuke");
		case maps::de_stmarc:    return _T("de_stmarc");
		case maps::de_sugarcane: return _T("de_sugarcane");
		case maps::de_train:     return _T("de_train");
		case maps::de_vertigo:   return _T("de_vertigo");
		case maps::de_zoo:       return _T("de_zoo");
	}
}

std::vector<std::filesystem::path> csgo::MaploadBckgChanger::get_custom_mapload_bckg_paths()
{
	std::vector<std::filesystem::path> paths;
	if (std::filesystem::directory_iterator it(csgo::constants::PATH_TO_DATA / MaploadBckgChanger::FOLDER_NAME); it->exists())
	{
		for (auto&& path : it)
			paths.push_back(path);
	}
	else
		std::tcout << _T("[~] No custom mapload background found.") << std::endl;

	return paths;
}