#include "pch.hpp"
#include "GraphicsSystem.hpp"

#include "Graphics/Vulkan/GraphicsLayerVulkan.hpp"
#include "Graphics/OpenGL/GraphicsLayerGL.hpp"
#include "Platform/Platform.hpp"

namespace tur
{
	void GraphicsSystem::Initialize(const ConfigData& configData, platform::Window& window)
	{
		switch (configData.graphicsSpecifications.api)
		{
			case GraphicsAPI::OPENGL:
				InitializeOpenGL(configData, window);
				break;

			case GraphicsAPI::VULKAN:
				InitializeVulkan(configData, window);
				break;
		}
	}

	void GraphicsSystem::InitializeOpenGL(const ConfigData& configData, platform::Window& window)
	{
		platform::gl::SetupOpenGLWindowing(window, configData.windowProperties, configData.graphicsSpecifications);
	
		m_GraphicsLayer = std::make_unique<gl::GraphicsLayerGL>(window);
		m_GraphicsLayer->Initialize();
	}

	void GraphicsSystem::InitializeVulkan(const ConfigData& configData, platform::Window& window)
	{
		platform::vulkan::SetupVulkanWindowing(window, configData.windowProperties);

		m_GraphicsLayer = std::make_unique<vulkan::GraphicsLayerVulkan>(window);
		m_GraphicsLayer->Initialize();
	}
}