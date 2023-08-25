#pragma once
#include "pch.h"
#include "System/Window/BaseWindowLoader.h"

namespace tur
{
	class WindowLoader : public BaseWindowLoader<WindowLoader>
	{
	public:
		bool Preload()
		{
			if (!glfwInit())
				return s_IsGLFWInitialized;

			s_IsGLFWInitialized = true;
			return true;
		}

		bool PostLoad() { return true; }

	private:
		static inline bool s_IsGLFWInitialized = false;
	};
}