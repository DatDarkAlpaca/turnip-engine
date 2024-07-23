#include "pch.hpp"
#include "GraphicsSystem.hpp"

#include "Graphics/OpenGL/OpenGLInitializer.hpp"
#include "Graphics/Vulkan/VulkanInitializer.hpp"

namespace tur
{
	void GraphicsSystem::Initialize(const ConfigSystem& configSystem, platform::Window& window)
	{
		switch (configSystem.GetGraphicsSpecification().api)
		{
		case GraphicsAPI::OPENGL:
			InitializeOpenGL(configSystem, window);
			break;

		case GraphicsAPI::VULKAN:
			InitializeVulkan(configSystem, window);
			break;
		}
	}

	void GraphicsSystem::InitializeOpenGL(const ConfigSystem& configSystem, platform::Window& window)
	{
		gl::OpenGLInitializer initializer(configSystem, window);
		initializer.Initialize();
	}

	void GraphicsSystem::InitializeVulkan(const ConfigSystem& configSystem, platform::Window& window)
	{
		vulkan::DefaultVulkanInitializer initializer(configSystem, window);
		initializer.Initialize();
	}
}