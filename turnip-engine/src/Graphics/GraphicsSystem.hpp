#pragma once
#include "Core/Config/ConfigSystem.hpp"
#include "Platform/Platform.hpp"

namespace tur
{
	class GraphicsSystem
	{
	public:
		void Initialize(const ConfigSystem& configSystem, platform::Window& window);

	private:
		void InitializeOpenGL(const ConfigSystem& configSystem, platform::Window& window);

		void InitializeVulkan(const ConfigSystem& configSystem, platform::Window& window);
	};
}