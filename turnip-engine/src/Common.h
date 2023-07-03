#pragma once
#include <numeric>

#ifdef _DEBUG
	#define TUR_DEBUG
#elif defined(NDEBUG)
	#define TUR_RELEASE
#endif

#ifdef TUR_DEBUG
	#define TUR_WRAP_DEBUG(x) x
#else
	#define TUR_WRAP_DEBUG(x)
#endif

#ifdef _WIN32
	#define TUR_PLATFORM_WINDOWS
#endif

#ifdef TUR_PLATFORM_WINDOWS
	#define TUR_BREAKPOINT() __debugbreak()
#else
	#define TUR_BREAKPOINT() __builtin_trap()
#endif

using U8  = std::uint8_t;
using U16 = std::uint16_t;
using U32 = std::uint32_t;
using U64 = std::uint64_t;