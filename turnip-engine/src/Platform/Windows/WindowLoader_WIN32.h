#pragma once
#ifdef TUR_WINDOWING_WINDOWS
#include "Platform/Windows/Window_WIN32.h"

namespace tur
{
	class WindowLoader
	{
	public:
		bool Preload() { return true; }
	};
}
#endif