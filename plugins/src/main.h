#pragma once

#ifdef WIN32	
	#include <windows.h>
#endif

#include "SDK/amx/amx.h"
#include "SDK/plugincommon.h"

typedef void(*logprintf_t)(const char* format, ...);
logprintf_t logprintf;