#pragma once
#include <numeric>

#include "Core/Defines.h"
#include "Core/NonCopyable.h"

#include "Core/Memory/Memory.h"
#include "Core/Logger/Logger.h"

#define BIND(function, argument) std::bind(function, argument, std::placeholders::_1)

namespace tur
{
	inline void __tur_assert(const char* condition, const char* message, const char* file, uint64_t line)
	{
		TUR_LOG_CRITICAL("Assert: {} failed. [{}]\nFile: {} [{}]", condition, message, file, line);
	}

#define TUR_ASSERT(condition, message)									\
	{																		\
		if(condition) { }													\
		else																\
		{																	\
			tur::__tur_assert(#condition, message, __FILE__, __LINE__);		\
		}																	\
	}

#define TUR_ASS(condition)																\
	{																						\
		if(condition) { }																	\
		else																				\
		{																					\
			tur::__tur_assert(#condition, "No message provided", __FILE__, __LINE__);		\
		}																					\
	}
}