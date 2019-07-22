// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "BackgroundChanger.hpp"
#include "Util.hpp"
#include "StringMacro.hpp"

/// [url] https://docs.microsoft.com/ru-ru/cpp/text/generic-text-mappings-in-tchar-h?view=vs-2019 [/url]
#include <tchar.h>
/// [url] https://en.cppreference.com/w/cpp/header/stdexcept [/url]
#include <stdexcept>
/// [url] https://en.cppreference.com/w/cpp/header/iostream [/url]
#include <iostream>
/// [url] http://en.cppreference.com/w/cpp/header/string [/url]
#include <string>

/*
std::thread csgo::BackgroundChanger::on_daytime_updater = std::thread([]()
	{	
		while (true)
		{
			std::time_t tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			std::tm* ptm = std::localtime_s(&tt);

			if (ptm->tm_hour >= 9 || ptm->tm_hour < 18)
			{

			}
			else
			{

			}

			std::this_thread::sleep_until(std::chrono::system_clock::from_time_t(mktime(ptm)));
		}
	});
	*/

void csgo::BackgroundChanger::backup()
{
	std::filesystem::path path_to_backup = csgo::constants::PATH_TO_BACKUP / BackgroundChanger::FOLDER_NAME;

	if (!std::filesystem::is_directory(path_to_backup))
	{
		std::filesystem::remove(path_to_backup);
		std::filesystem::create_directories(path_to_backup);
	}

	path_to_backup /= BackgroundChanger::FILE_TO_CHANGE;

	if (!std::filesystem::exists(path_to_backup))
	{
		auto file_to_change = csgo::constants::PATH_TO_CSGO / BackgroundChanger::PATH_TO_BCKG / BackgroundChanger::FILE_TO_CHANGE;
		if (!std::filesystem::exists(file_to_change))
		{
			std::tcout << _T("[~] Cannot backup file(File not found).") << std::endl;

			return;
		}
			
		std::filesystem::rename(file_to_change, path_to_backup);

		std::tcout << _T("[+] Successfully backuped.") << std::endl;
	}
	else
		std::tcout << _T("[-] Backup is not needed.") << std::endl;
}

bool csgo::BackgroundChanger::add(std::filesystem::path bckg)
{
	if (!std::filesystem::exists(bckg))
		throw std::invalid_argument("[!] Could not find new background.");

	std::filesystem::path path_to_bckg_data = csgo::constants::PATH_TO_DATA / BackgroundChanger::FOLDER_NAME;
	if (!std::filesystem::is_directory(path_to_bckg_data))
	{
		std::filesystem::remove(path_to_bckg_data);
		std::filesystem::create_directories(path_to_bckg_data);
	}
	std::filesystem::rename(bckg, path_to_bckg_data / bckg.filename());

	std::tcout << _T("[+] New background successfully added(") << path_to_bckg_data / bckg.filename() << _T(").") << std::endl;

	return true;
}

bool csgo::BackgroundChanger::change(std::filesystem::path new_bckg)
{
	if (!std::filesystem::exists(new_bckg))
		throw std::invalid_argument("[!] Could not find new background.");

	BackgroundChanger::backup();

	if (!std::filesystem::copy_file(new_bckg, csgo::constants::PATH_TO_CSGO / BackgroundChanger::PATH_TO_BCKG / BackgroundChanger::FILE_TO_CHANGE, std::filesystem::copy_options::overwrite_existing))
		throw std::runtime_error("[!] Could not copy background.");

	std::tcout << _T("[+] Background successfully changed.") << std::endl;

	return true;
}

bool csgo::BackgroundChanger::restore()
{
	std::filesystem::path path_to_backup = csgo::constants::PATH_TO_BACKUP / BackgroundChanger::FOLDER_NAME / BackgroundChanger::FILE_TO_CHANGE;
	if (!std::filesystem::exists(path_to_backup))
	{
		std::tcout << _T("[~] Background not found. Cannot restore(") << path_to_backup << _T(").") << std::endl;

		return false;
	}

	std::filesystem::path prev_bckg = csgo::constants::PATH_TO_CSGO / BackgroundChanger::PATH_TO_BCKG / BackgroundChanger::FILE_TO_CHANGE;
	if(std::filesystem::exists(prev_bckg))
		std::filesystem::remove(prev_bckg);

	std::filesystem::rename(path_to_backup, prev_bckg);

	std::tcout << _T("[+] Background successfully restored(") << prev_bckg << _T(").") << std::endl;

	return true;
}

std::vector<std::filesystem::path> csgo::BackgroundChanger::get_custom_bckg_paths()
{
	std::vector<std::filesystem::path> paths;
	if (std::filesystem::directory_iterator it(csgo::constants::PATH_TO_DATA / BackgroundChanger::FOLDER_NAME); it->exists())
	{
		for (auto&& path : it)
			paths.push_back(path);
	}
	else
		std::tcout << _T("[~] No custom background found.") << std::endl;

	return paths;
}
