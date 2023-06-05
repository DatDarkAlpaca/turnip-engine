#pragma once
#include "Debug_Definition.h"
#include "PlatformDetection.h"
#include "Types.h"
#include "System/Logger/Logger.h"

#ifdef TUR_WINDOWS
	#include <intrin.h>
	#define DEBUG_BREAK() __debugBreak()
#else
	#define DEBUG_BREAK() __buildin_trap()
#endif

#ifdef TUR_DEBUG

	inline void __assertion_failed(const char* expression, const char* message, const char* filename, tur::U32 line)
	{
		TUR_CORE_CRITICAL("Assertion Failed: {} | {}\nFile:{} ({})", expression, message, filename, line);
	}

	#define TUR_ASSERT(expression, message)									\
	{																		\
		if(expression) { }														\
		else																\
		{																	\
			__assertion_failed(#expression, message, __FILE__, __LINE__);	\
			DEBUG_BREAK();													\
		}																	\
	}

	#define TUR_ASSERT_CORE(expression)									\
	{																	\
		if(expression) { }												\
		else															\
		{																\
			__assertion_failed(#expression, "", __FILE__, __LINE__);	\
			DEBUG_BREAK();												\
		}																\
	}

#else
	#define TUR_ASSERT(expression, message)

	#define TUR_ASSERT_CORE(expression)

#endif