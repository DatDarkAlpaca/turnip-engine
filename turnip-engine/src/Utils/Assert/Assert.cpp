#include "pch.h"
#include "Utils/Logger/Logger.h"

namespace tur
{
	void __assert(const char* condition, const char* message, const char* file, U32 line)
	{
		TUR_CORE_CRITICAL("Assert: {} failed. [{}]\nFile: {} [{}]", condition, message, file, line);
	}
}