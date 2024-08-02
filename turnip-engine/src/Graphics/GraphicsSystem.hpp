#pragma once
#include "Core/Config/ConfigData.hpp"
#include "Platform/Platform.hpp"

namespace tur
{
	class GraphicsSystem
	{
	public:
		void Initialize(const ConfigData& config, platform::Window& window);

	private:
		void InitializeOpenGL(const ConfigData& config, platform::Window& window);

		void InitializeVulkan(const ConfigData& config, platform::Window& window);
	};
}