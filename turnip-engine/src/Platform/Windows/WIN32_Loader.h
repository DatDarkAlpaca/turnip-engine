#pragma once
#include "Core/Window/Window.h"
#include "Graphics/API/IGraphicsAPI_Loader.h"

namespace tur
{
	class WIN32_Loader_OGL : public IGraphicsAPI_Loader
	{ 
	public:
		void PreInitialize() override { TUR_CORE_WARN("An OpenGL loader is not available for Win32 platforms yet."); }

		void PostInitialize(Window* window) override { TUR_CORE_WARN("An OpenGL loader is not available for Win32 platforms yet."); }
	};

	class WIN32_Loader_D3D11 : public IGraphicsAPI_Loader
	{
	public:
		void PreInitialize() override { TUR_CORE_WARN("An D3D11 loader is not available for Win32 platforms yet."); }

		void PostInitialize(Window* window) override { TUR_CORE_WARN("An D3D11 loader is not available for Win32 platforms yet."); }
	};
}