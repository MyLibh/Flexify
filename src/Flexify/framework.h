// header.h : include file for standard system include files,
// or project specific include files
//

#pragma once

#include "..\..\res\Flexify\targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <commdlg.h>
#include <ShlObj.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
// C++ Header Files
#include <iostream>
#include <string>
#include <filesystem>
// My Header Files
#include "..\\CSGOChanger\StringMacro.hpp"

using string = std::basic_string<TCHAR>;