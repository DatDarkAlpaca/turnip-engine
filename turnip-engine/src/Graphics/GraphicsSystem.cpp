#include "pch.hpp"
#include "GraphicsSystem.hpp"

#include "Graphics/OpenGL/OpenGLInitializer.hpp"
#include "Graphics/Vulkan/VulkanInitializer.hpp"

namespace tur
{
	void GraphicsSystem::Initialize(const ConfigData& config, platform::Window& window)
	{
		switch (config.graphicsSpecifications.api)
		{
		case GraphicsAPI::OPENGL:
			InitializeOpenGL(config, window);
			break;

		case GraphicsAPI::VULKAN:
			InitializeVulkan(config, window);
			break;
		}
	}

	void GraphicsSystem::InitializeOpenGL(const ConfigData& config, platform::Window& window)
	{
		gl::OpenGLInitializer initializer(config, window);
		initializer.Initialize();
	}

	void GraphicsSystem::InitializeVulkan(const ConfigData& config, platform::Window& window)
	{
		vulkan::DefaultVulkanInitializer initializer(config, window);
		initializer.Initialize();
	}
}