#pragma once
#include "PlatformDetection.h"

#ifdef TUR_WINDOWS
	#ifdef TUR_EXPORT
		#define TUR_API __declspec(dllexport)
	#else
		#define TUR_API __declspec(dllimport)
#endif

#else
	#ifdef TUR_IMPORT
		#define TUR_API
	#else
		#define TUR_API
	#endif
#endif