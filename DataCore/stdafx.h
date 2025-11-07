// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// TODO: reference additional headers your program requires here
#include <stdio.h>
#include <winsock2.h>
#include <MSWSock.h>

#pragma comment(lib, "ws2_32")

#endif // _WIN32
#include "Tool/TLog.h"