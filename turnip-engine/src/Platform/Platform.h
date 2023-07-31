#pragma once
#include "Common.h"

#if defined(TUR_PLATFORM_WINDOWS)

#include "Platform/Windows/WIN32_Window.h"
#include "Platform/Windows/WIN32_Loader.h"
#include "Platform/OpenGL/OGL_API.h"

namespace tur
{
	using Window = WIN32_Window;
	using OGL_GraphicsLoader = WIN32_Loader_OGL;
	using D3D11_GraphicsLoader = WIN32_Loader_D3D11;

	using OGL_API = OGL_API_WIN32;
}

#elif defined(TUR_WINDOWING_GLFW)

#include "Platform/GLFW/GLFW_Window.h"
#include "Platform/GLFW/GLFW_Loader.h"
#include "Platform/OpenGL/OGL_API.h"

namespace tur
{
	using Window = GLFW_Window;
	using OGL_GraphicsLoader = GLFW_Loader_OGL;
	using D3D11_GraphicsLoader = GLFW_Loader_D3D11;
	using OGL_API = OGL_API_GLFW;
}

#endif