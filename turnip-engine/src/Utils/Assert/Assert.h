#pragma once
#include "Common.h"

namespace tur
{
	void __assert(const char* condition, const char* message, const char* file, U32 line);
}

#define TUR_ASSERT(condition, message)									\
{																		\
	if(condition) { }													\
	else																\
	{																	\
		tur::__assert(#condition, message, __FILE__, __LINE__);			\
		TUR_BREAKPOINT();												\
	}																	\
}

#define TUR_ASS(condition)																\
{																						\
	if(condition) { }																	\
	else																				\
	{																					\
		tur::__assert(#condition, "No message provided", __FILE__, __LINE__);			\
		TUR_BREAKPOINT();																\
	}																					\
}