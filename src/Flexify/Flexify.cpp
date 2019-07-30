// Flexify.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Flexify.h"
#include "..\..\src\CSGOChanger\Util.hpp"
#include "..\..\src\CSGOChanger\BackgroundChanger.hpp"
#include "..\..\src\CSGOChanger\MaploadBckgChanger.hpp"

#define MBOX_INFO(info)  MessageBox(hWnd, _T(info), _T("Info"), MB_OK | MB_ICONINFORMATION)
#define MBOX_ERROR()     MessageBox(hWnd, GetLastErrorMessage().c_str(), _T("Error"), MB_OK | MB_ICONERROR)

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                               
TCHAR szTitle[MAX_LOADSTRING];           
TCHAR szWindowClass[MAX_LOADSTRING];           

string GetLastErrorMessage();
ATOM                _RegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK AboutDialog(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK RestoreLoadmapDialog(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK ChangeLoadmapDialog(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK SettingsDialog(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_FLEXIFY, szWindowClass, MAX_LOADSTRING);
    _RegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FLEXIFY));

	MSG msg{};

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

string GetLastErrorMessage()
{
	LPTSTR psz{ };
	const DWORD cchMsg = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPTSTR>(&psz), 0, NULL);
	if (cchMsg > 0)
	{
		auto deleter = [](void* p) { HeapFree(GetProcessHeap(), 0, p); };
		std::unique_ptr<TCHAR, decltype(deleter)> ptrBuffer(psz, deleter);

		return string(ptrBuffer.get(), cchMsg);
	}
	else
	{
		auto error_code{ GetLastError() };

		throw std::system_error(error_code, std::system_category(), "Failed to retrieve error message string.");
	}
}

ATOM _RegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex     = { sizeof(WNDCLASSEX) };
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FLEXIFY));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName   = MAKEINTRESOURCE(IDC_FLEXIFY);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; 

   HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
      return FALSE;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

auto OpenFileDialog(HWND owner, TCHAR const* filter, TCHAR const* ext, TCHAR const* initial_dir)
{
	TCHAR szFileName[MAX_PATH]{};

	OPENFILENAME ofn    = { sizeof(ofn) };
	ofn.hwndOwner       = owner;
	ofn.lpstrFilter     = filter;
	ofn.lpstrFile       = szFileName;
	ofn.nMaxFile        = MAX_PATH;
	ofn.lpstrInitialDir = initial_dir;
	ofn.Flags           = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt     = ext;

	if(GetOpenFileName(&ofn))
		return std::tstring{ szFileName };

	return std::tstring{};
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
			case ID_FILE_SETTINGS:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_SETTINGS_DLG), hWnd, SettingsDialog);
				break;

            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, AboutDialog);
                break;

			case ID_TOOLS_CONVERTER:
			{
				std::filesystem::path path = OpenFileDialog(hWnd, _T("Image files (*.jpg, *.png)\0*.jpg;*.png\0Video files(*.mp4)\0*.mp4\0\0"), nullptr, nullptr);
				if (!path.empty())
				{
					if (auto ext = path.extension(); ext == _T(".mp4"))
						csgo::mp4_to_webm(path); 
				}

				break;
			}

			case ID_BACKGROUND_ADD:
			{
				std::filesystem::path path = OpenFileDialog(hWnd, _T("Video files (*.webm)\0*.webm\0\0"), _T("webm"), nullptr);
				if(!path.empty())
				{
					if (csgo::BackgroundChanger::add(path))
						MBOX_INFO("Backgroound successfully added");
					else
						MBOX_ERROR();
				}

				break;
			}

			case ID_BACKGROUND_CHANGE:
			{
				std::filesystem::path path = OpenFileDialog(hWnd, _T("Video files (*.webm)\0*.webm\0\0"), _T("webm"), (csgo::constants::PATH_TO_DATA / "bckg").string().c_str());
				if (!path.empty())
				{
					if(csgo::BackgroundChanger::change(path))
						 MBOX_INFO("Backgroound successfully changed");
					else
						MBOX_ERROR();
				}

				break;
			}

			case ID_BACKGROUND_RESTORE:
			{
				if (csgo::BackgroundChanger::restore())
					MBOX_INFO("Backgroound successfully restored");
				else
					MBOX_ERROR();

				break;
			}

			case ID_MAPLOAD_ADD:
			{
				std::filesystem::path path = OpenFileDialog(hWnd, _T("Image files(*.jpg)\0*.jpg\0\0"), _T("jpg"), nullptr);
				if (!path.empty())
				{
					if(csgo::MaploadBckgChanger::add(path))
						MBOX_INFO("Mapload successfully added");
					else
						MBOX_ERROR();
				}

				break;
			}

			case ID_MAPLOAD_CHANGE:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_CHANGE_LOADMAP_DLG), hWnd, ChangeLoadmapDialog);
				break;

			case ID_MAPLOAD_RESTORE:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_RESTORE_LOADMAP_DLG), hWnd, RestoreLoadmapDialog);
				break;

            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;

            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
           
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
		return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK AboutDialog(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hWnd, LOWORD(wParam));

			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK RestoreLoadmapDialog(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	HWND hCombo = GetDlgItem(hWnd, IDC_RESTORE_LOADMAP_DLG_COMBO);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		for (auto map : csgo::constants::MAPS)
			SendMessage(hCombo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(const_cast<TCHAR*>(map.data())));

		return (INT_PTR)TRUE;
	}

	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_RESTORE_LOADMAP_DLG_COMBO && HIWORD(wParam) == CBN_SELENDOK)
		{
			auto map = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
			if (csgo::MaploadBckgChanger::restore(static_cast<csgo::MaploadBckgChanger::maps>(map)))
				MBOX_INFO((string{ "Successfully restored(" } +csgo::constants::MAPS[map].data() + ')').c_str());
			else
				MBOX_ERROR();

			EndDialog(hWnd, LOWORD(wParam));

			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hWnd, LOWORD(wParam));

			return (INT_PTR)TRUE;
		}
		break;
	}

	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK ChangeLoadmapDialog(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	HWND hCombo = GetDlgItem(hWnd, IDC_CHANGE_LOADMAP_DLG_COMBO);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		for (auto map : csgo::constants::MAPS)
			SendMessage(hCombo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(const_cast<TCHAR*>(map.data())));

		return (INT_PTR)TRUE;
	}

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_CHANGE_LOADMAP_DLG_BUTTON_PATH:
		{
			auto path = OpenFileDialog(hWnd, _T("Image files (*.jpg)\0*.jpg\0\0"), _T("jpg"), (csgo::constants::PATH_TO_DATA / "mapload_bckg").string().c_str());
			if (!path.empty())
			{
				SetDlgItemText(hWnd, IDC_CHANGE_LOADMAP_DLG_STATIC_PATH, path.c_str());

				return (INT_PTR)TRUE;
			}

			break;
		}
		case IDD_CHANGE_LOADMAP_BUTTON_OK:
		{
			TCHAR path[MAX_PATH]{};
			GetDlgItemText(hWnd, IDC_CHANGE_LOADMAP_DLG_STATIC_PATH, path, MAX_PATH);
			if (!_tcslen(path))
			{
				SetDlgItemText(hWnd, IDC_CHANGE_LOADMAP_DLG_STATIC_ERROR, _T("[!] No path specified"));

				break;
			}

			auto map = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
			if (map == CB_ERR)
			{
				SetDlgItemText(hWnd, IDC_CHANGE_LOADMAP_DLG_STATIC_ERROR, _T("[!] No map specified"));

				break;
			}

			if (csgo::MaploadBckgChanger::change(static_cast<csgo::MaploadBckgChanger::maps>(map), path))
				MBOX_INFO((string{ "Successfully changed(" } + csgo::constants::MAPS[map].data() + ')').c_str());
			else
				MBOX_ERROR();

			EndDialog(hWnd, LOWORD(wParam));

			return (INT_PTR)TRUE;
		}
		case IDCANCEL:
		case IDD_CHANGE_LOADMAP_BUTTON_CANCEL:
			EndDialog(hWnd, LOWORD(wParam));
			return (INT_PTR)TRUE;

		default:
			break; 
		}

	default:
		break;
	}

	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK SettingsDialog(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		SetDlgItemText(hWnd, IDC_SETTINGS_DLG_STATIC, csgo::constants::PATH_TO_CSGO.string().c_str());
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_SETTINGS_DLG_BUTTON:
		{
			TCHAR szDir[MAX_PATH]{};
			BROWSEINFO bi{};
			bi.hwndOwner      = hWnd;
			bi.pszDisplayName = szDir; 
			bi.lpszTitle      = _T("Please, select a folder with CSGO"); 
			bi.iImage         = -1;

			LPITEMIDLIST lpItem = SHBrowseForFolder(&bi);
			if (lpItem != NULL)
			{
				SHGetPathFromIDList(lpItem, szDir);

				WritePrivateProfileString(_T("CSGO"), _T("path"), szDir, csgo::constants::PATH_TO_INI.generic_string().c_str());

				MBOX_INFO("Restart the app");

				EndDialog(hWnd, LOWORD(wParam));
			}

			break;
		}
		case IDCANCEL:
			EndDialog(hWnd, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}

	default:
		break;
	}

	return (INT_PTR)FALSE;
}