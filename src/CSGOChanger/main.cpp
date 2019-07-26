// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "BackgroundChanger.hpp"
#include "MaploadBckgChanger.hpp"

#include "StringMacro.hpp"
#include "StringUtil.hpp"
#include "Util.hpp"

/// [url] https://en.cppreference.com/w/cpp/header/iostream [/url]
#include <iostream>

constexpr std::tstring_view VERSION(_T("20190717"));

signed main()
{
	try
	{
		while (!GetAsyncKeyState(VK_ESCAPE))
		{
#ifdef _WIN64
			std::tcout << _T("CSGOChanger x64 [Version ") << VERSION << _T("]") << std::endl;
#else
			std::tcout << _T("CSGOChanger x86 [Version ") << VERSION << _T("]") << std::endl;
#endif
			std::tcout << _T("Copyright(c) 2019 MyLibh. All rights reserved.") << std::endl << std::endl;

			std::tcout << _T("1. BackgroundChanger") << std::endl;
			std::tcout << _T("2. MaploadBckgChanger") << std::endl;
			std::tcout << _T("[ESC] Exit") << std::endl;

			int act{};
			std::tcin >> act;
			if (act == 1)
			{
				std::tcout << _T("\n[BackgroundChanger]") << std::endl;
				std::tcout << _T("1. Convert") << std::endl;
				std::tcout << _T("2. Add") << std::endl;
				std::tcout << _T("3. Change") << std::endl;
				std::tcout << _T("4. Restore") << std::endl;
				std::tcout << _T("5. Show") << std::endl;
				std::tcout << _T("6. Go to menu") << std::endl;

				int act2{};
				std::tcin >> act2;
				if (act2 == 1)
				{
					std::tcout << _T("[+] Enter path to .mp4 file:") << std::endl;
					std::tstring str;
					std::getline(std::tcin, str);
					std::getline(std::tcin, str);

					csgo::mp4_to_webm(str);
				}
				else if (act2 == 2)
				{
					std::tcout << _T("[+] Enter path to .webm file:") << std::endl;
					std::tstring str;
					std::getline(std::tcin, str);
					std::getline(std::tcin, str);

					csgo::BackgroundChanger::add(str);
				}
				else if (act2 == 3)
				{
					std::tcout << _T("[+] Enter path to new background:") << std::endl;
					std::tstring str;
					std::getline(std::tcin, str);
					std::getline(std::tcin, str);

					csgo::BackgroundChanger::change(str);
				}
				else if (act2 == 4)
					csgo::BackgroundChanger::restore();
				else if (act2 == 5)
				{
					auto paths = csgo::BackgroundChanger::get_custom_bckg_paths();
					for (auto&& p : paths)
						std::tcout << p << std::endl;
				}
				else if (act2 == 6)
					continue;
				else
				{
					std::tcout << _T("[!] Wrong action") << std::endl;
					
					continue;
				}
			}
			else if (act == 2)
			{
				std::tcout << _T("\n[MaploadBckgChanger]") << std::endl;
				std::tcout << _T("1. Add") << std::endl;
				std::tcout << _T("2. Change") << std::endl;
				std::tcout << _T("3. Restore") << std::endl;
				std::tcout << _T("4. Show") << std::endl;
				std::tcout << _T("5. Go to menu") << std::endl;

				int act2{};
				std::tcin >> act2;
				if (act2 == 1)
				{
					std::tcout << _T("[+] Enter path to file:") << std::endl;
					std::tstring str;
					std::getline(std::tcin, str);
					std::getline(std::tcin, str);

					csgo::MaploadBckgChanger::add(str);
				}
				else if (act2 == 2)
				{
					std::tcout << _T("[+] Enter path to file:") << std::endl;
					std::tstring str;
					std::getline(std::tcin, str);
					std::getline(std::tcin, str);
					std::tcout << str << std::endl;
					std::tcout << _T("[+] Enter map:") << std::endl;
					for (int i{}; i < csgo::MaploadBckgChanger::maps::NUM; ++i)
						std::tcout << i << _T(". ") << csgo::MaploadBckgChanger::get_map_name_by_id(static_cast<csgo::MaploadBckgChanger::maps>(i)) << std::endl;

					int act3{};
					std::tcin >> act3;

					csgo::MaploadBckgChanger::change(static_cast<csgo::MaploadBckgChanger::maps>(act3), str);
				}
				else if (act2 == 3)
				{
					std::tcout << _T("[+] Enter map:") << std::endl;
					for (int i{}; i < csgo::MaploadBckgChanger::maps::NUM; ++i)
						std::tcout << i << _T(". ") << csgo::MaploadBckgChanger::get_map_name_by_id(static_cast<csgo::MaploadBckgChanger::maps>(i)) << std::endl;

					int act3{};
					std::tcin >> act3;

					csgo::MaploadBckgChanger::restore(static_cast<csgo::MaploadBckgChanger::maps>(act3));
				}
				else if (act2 == 4)
				{
					auto paths = csgo::MaploadBckgChanger::get_custom_mapload_bckg_paths();
					for (auto&& p : paths)
						std::tcout << p << std::endl;
				}
				else if (act2 == 5)
					continue;
				else
				{
					std::tcout << _T("[!] Wrong action") << std::endl;

					continue;
				}
				
			}
			else
				std::tcout << _T("[!] Wrong action") << std::endl;
		}
	}
	catch (std::exception ex)
	{
		std::cout << "[FATAL] " << ex.what() << std::endl;
	}

	system("pause");
	return 0;
}