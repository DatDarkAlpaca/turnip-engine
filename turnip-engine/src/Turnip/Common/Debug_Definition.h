#pragma once

#ifdef _DEBUG
	#define TUR_DEBUG
#elif defined(NDEBUG)
	#define TUR_RELEASE
#endif