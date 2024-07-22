#include "pch.hpp"
#include "WindowSystem.hpp"

#include "Graphics/OpenGL/OpenGLInitializer.hpp"
#include "Graphics/Vulkan/VulkanInitializer.hpp"

namespace tur
{
	void WindowSystem::Initialize(const ConfigSystem& configSystem)
	{
		switch (configSystem.GetGraphicsSpecification().api)
		{
			case GraphicsAPI::OPENGL:
				InitializeOpenGL(configSystem);
				break;

			case GraphicsAPI::VULKAN:
				InitializeVulkan(configSystem);
				break;
		}
	}

	void WindowSystem::SetEventCallback(const EventCallback& callback)
	{
		m_Window.SetEventCallback(callback);
	}

	void WindowSystem::Shutdown()
	{
		m_Window.Shutdown();
	}

	void WindowSystem::InitializeOpenGL(const ConfigSystem& configSystem)
	{
		gl::OpenGLInitializer initializer(configSystem, m_Window);
		initializer.Initialize();
	}

	void WindowSystem::InitializeVulkan(const ConfigSystem& configSystem)
	{
		vulkan::DefaultVulkanInitializer initializer(configSystem, m_Window);
		initializer.Initialize();
	}
}
