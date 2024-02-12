#pragma once

#if defined(_DEBUG) && !defined(TUR_DEBUG)
	#define TUR_DEBUG
#elif defined(NDEBUG) && !defined(TUR_RELEASE)
	#define TUR_RELEASE
#endif

#ifdef TUR_PLATFORM_WIN32
	#define TUR_BREAKPOINT() __debugbreak()
#elif defined(__linux__)
	#define TUR_PLATFORM_LINUX
	#define TUR_BREAKPOINT() __builtin_trap()
#elif defined(__APPLE__)
	#define TUR_PLATFORM_APPLE
	#define TUR_BREAKPOINT() __builtin_trap()
#elif defined(__ANDROID__)
	#define TUR_PLATFORM_ANDROID
	#define TUR_BREAKPOINT() __builtin_trap()
#else
	#error Unsupported platform
#endif