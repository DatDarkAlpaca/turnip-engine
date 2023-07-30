#pragma once
#include "Core/Window/Window.h"
#include "Graphics/API/IGraphicsAPI_Loader.h"

namespace tur
{
	class WIN32_Loader_OGL : public IGraphicsAPI_Loader
	{ 
	public:
		void PreInitialize() override;

		void PostInitialize(Window* window, IGraphicsAPI* api) override;

	private:
		void CreateDummyContext();
	};

	class WIN32_Loader_D3D11 : public IGraphicsAPI_Loader
	{
	public:
		void PreInitialize() override { TUR_CORE_WARN("An D3D11 loader is not available for Win32 platforms yet."); }

		void PostInitialize(Window*, IGraphicsAPI*) override { TUR_CORE_WARN("An D3D11 loader is not available for Win32 platforms yet."); }
	};
}