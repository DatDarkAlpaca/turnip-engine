#pragma once
#include <numeric>

// Debug macros:
#ifdef _DEBUG
	#define TUR_DEBUG
	#define TUR_WRAP_DEBUG(x) x
#elif defined(NDEBUG)
	#define TUR_RELEASE
	#define TUR_WRAP_DEBUG(x)
#endif

// Platform-specific macros:
#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
	#define TUR_PLATFORM_WINDOWS
	#define TUR_WINDOWING_WINDOWS
	#define TUR_BREAKPOINT() __debugbreak()

#elif defined(__linux__)
	#define TUR_PLATFORM_LINUX
	#define TUR_WINDOWING_GLFW
	#define TUR_BREAKPOINT() __builtin_trap()

#elif defined(__APPLE__ ) || defined(__MACH__)
	#define TUR_PLATFORM_MACOS
	#define TUR_WINDOWING_GLFW
	#define TUR_BREAKPOINT() __builtin_trap()

#endif

#if 1
	#undef TUR_PLATFORM_WINDOWS
	#undef TUR_WINDOWING_WINDOWS
	#define TUR_WINDOWING_GLFW
#endif

#define BIND_1(function, argument) std::bind(function, argument, std::placeholders::_1)

using U8  = std::uint8_t;
using U16 = std::uint16_t;
using U32 = std::uint32_t;
using U64 = std::uint64_t;