#pragma once
#include <numeric>

namespace tur
{
	struct BackendVersion
	{
		uint32_t major = 0xFFFFFFFF, minor = 0xFFFFFFFF, patch = 0xFFFFFFFF, extra = 0xFFFFFFFF;
	};
}