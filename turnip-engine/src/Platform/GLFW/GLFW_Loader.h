#pragma once
#include "Core/Window/Window.h"
#include "Graphics/API/IGraphicsAPI_Loader.h"

namespace tur
{
	class GLFW_Loader_OGL : public IGraphicsAPI_Loader
	{
	public:
		void PreInitialize() override;

		void PostInitialize(Window* window, IGraphicsAPI* api) override;
	};

	class GLFW_Loader_D3D11: public IGraphicsAPI_Loader
	{
	public:
		void PreInitialize() override { TUR_CORE_WARN("An OpenGL loader is not available for GLFW windows yet."); }

		void PostInitialize(Window*, IGraphicsAPI*) override { TUR_CORE_WARN("An OpenGL loader is not available for GLFW windows yet."); }
	};
}