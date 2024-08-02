#include "pch.hpp"
#include "UIBackend.hpp"

#include "Platform/GLFW/OpenGL/OpenGLUI.hpp"

namespace tur
{
    tur_unique<UIBackend> tur::CreateUIBackend(const ConfigData& config, platform::Window& window)
    {
		switch (config.graphicsSpecifications.api)
		{
			case GraphicsAPI::OPENGL:
				return tur::MakeUnique<OpenGLUI>();

			case GraphicsAPI::VULKAN:
				TUR_LOG_CRITICAL("[UIView]: Not yet implemented for Vulkan.");
				break;

			default:
				TUR_LOG_CRITICAL("[UIView]: Invalid graphics API.");
		}

		return nullptr;
    }
}