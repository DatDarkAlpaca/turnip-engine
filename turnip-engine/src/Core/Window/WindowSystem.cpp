#include "pch.hpp"
#include "WindowSystem.hpp"

namespace tur
{
	void WindowSystem::Initialize(const WindowProperties& properties, const GraphicsSpecification& graphicsSpecification)
	{
		switch (graphicsSpecification.api)
		{
			case GraphicsAPI::OPENGL:
				InitializeOpenGL(properties, graphicsSpecification);
				break;

			case GraphicsAPI::VULKAN:
				InitializeVulkan(properties, graphicsSpecification);
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

	void WindowSystem::InitializeOpenGL(const WindowProperties& properties, const GraphicsSpecification& graphicsSpecification)
	{
		platform::gl::SetupOpenGLWindowing(m_Window, properties, graphicsSpecification);
	}

	void WindowSystem::InitializeVulkan(const WindowProperties& properties, const GraphicsSpecification& graphicsSpecification)
	{
		platform::vulkan::SetupVulkanWindowing(m_Window, properties);

		/*if (initializer)
		{
			initializer->Initialize(renderDevice.get());
			break;
		}

		vulkan::DefaultVulkanInitializer vulkanInitializer(specification);
		vulkanInitializer.Initialize(renderDevice.get());*/
	}
}
