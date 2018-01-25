// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string>

using namespace std;

static void debug(std::string log)
{
	OutputDebugStringA(log.c_str());
	OutputDebugStringA("\n");
}
//Logs a string from a form of char array
static void debug(char a[])
{
	OutputDebugStringA(a);
	OutputDebugStringA("\n");
}

static void debug(char a[], std::string log)
{
	OutputDebugStringA(a);
	OutputDebugStringA(" : ");
	OutputDebugStringA(log.c_str());
	OutputDebugStringA("\n");
}


// TODO: reference additional headers your program requires here
