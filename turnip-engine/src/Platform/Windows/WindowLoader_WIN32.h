#pragma once
#include "pch.h"
#ifdef TUR_WINDOWING_WINDOWS
#include "System/Window/BaseWindowLoader.h"

namespace tur
{
	class WindowLoader_WIN32 : public BaseWindowLoader<WindowLoader_WIN32>
	{
	public:
		bool Preload() { return true; }

		bool PostLoad() { return true; }

	private:
		static inline bool s_IsGLFWInitialized = false;
	};
}
#endif