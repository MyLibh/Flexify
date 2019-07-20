// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util.hpp"
#include "StringMacro.hpp"
#include "StringUtil.hpp"

/// [url] https://docs.microsoft.com/en-us/windows/win32/api/shlobj/ [/url]
#include <ShlObj.h>
/// [url] https://en.cppreference.com/w/cpp/header/string [/url]
#include <string>
/// [url] https://en.cppreference.com/w/cpp/header/iostream [/url]
#include <iostream>

namespace
{
	///====================================================================================================================================
	/// <summary>   Trying to parse .ini file, starts OFD for folders overwise. </summary>
	///
	/// <remarks>	MyLibh, 21.07.2019. </remarks>
	///
	/// <returns>	Path to the csgo or zero-string. </returns>
	///====================================================================================================================================
	std::filesystem::path get_csgo_installation_path()
	{
		{
			std::tcout << _T("[+] Parsing .ini file.") << std::endl;

			TCHAR path[MAX_PATH]{};
			GetPrivateProfileString(_T("CSGO"), _T("path"), nullptr, path, MAX_PATH, csgo::constants::PATH_TO_INI.string().c_str());

			if (_tcslen(path))
				return path;

			std::tcout << _T("[-] Parsing failed.") << std::endl; 
		}

		TCHAR szDir[MAX_PATH]{};
		BROWSEINFO bi{};
		bi.pszDisplayName = szDir;
		bi.lpszTitle = _T("Please, select a folder with CSGO");
		bi.iImage = -1;

		LPITEMIDLIST lpItem = SHBrowseForFolder(&bi);
		if (lpItem != NULL)
		{
			SHGetPathFromIDList(lpItem, szDir);

			WritePrivateProfileString(_T("CSGO"), _T("path"), szDir, csgo::constants::PATH_TO_INI.generic_string().c_str());

			return szDir;
		}

		return _T("");
	}

	///====================================================================================================================================
	/// <summary>   Gets path to Documets folder. </summary>
	///
	/// <remarks>	MyLibh, 21.07.2019. </remarks>
	///
	/// <returns>	Path to the Documents. </returns>
	///====================================================================================================================================
	std::filesystem::path get_documents_path()
	{
		std::filesystem::path path_to_documetns;
		if (PWSTR tmp; SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, 0LLU, nullptr, &tmp)))
		{
			path_to_documetns = tmp;

			CoTaskMemFree(tmp);
		}

		return path_to_documetns;
	}
} // anonumos namespace

namespace csgo
{ 
	namespace constants
	{
		const std::filesystem::path PATH_TO_DOCUMENTS = get_documents_path();
		
		const std::filesystem::path PATH_TO_INI    = PATH_TO_DOCUMENTS / APP_FOLDER_NAME / INI_FILENAME;
		const std::filesystem::path PATH_TO_DATA   = PATH_TO_DOCUMENTS / APP_FOLDER_NAME / DATA_FOLDER_NAME;
		const std::filesystem::path PATH_TO_BACKUP = PATH_TO_DOCUMENTS / APP_FOLDER_NAME / BACKUP_FOLDER_NAME;
		const std::filesystem::path PATH_TO_BIN    = PATH_TO_DOCUMENTS / APP_FOLDER_NAME / BIN_FOLDER_NAME;

		const std::filesystem::path PATH_TO_CSGO = get_csgo_installation_path();
	} // namespace constants

	void mp4_to_webm(std::filesystem::path input)
	{
		auto output = constants::PATH_TO_DATA / "bckg" / input.filename();
		output.replace_extension(".webm");

		std::tstring cmd = constants::PATH_TO_BIN.string() + _T("\\") + _T("ffmpeg -i ") + _T('"') + input.string() + _T('"')  + _T(" -c:v libvpx-vp9 -crf 30 -b:v 0 -b:a 128k -c:a libopus ") + _T('"') + output.string() + _T('"');
		system(cmd.c_str());
	}
} // namespace csgo