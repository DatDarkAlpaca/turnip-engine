#pragma once
#include "Core/Config/ConfigData.hpp"
#include "Platform/Platform.hpp"

namespace tur::gl
{
	class OpenGLInitializer
	{
	public:
		explicit OpenGLInitializer(const ConfigData& config, platform::Window& window)
			: r_Window(window)
			, m_ConfigData(config)
		{

		}

	public:
		void Initialize()
		{
			auto& windowProperties = m_ConfigData.windowProperties;
			auto& graphicsSpecification = m_ConfigData.graphicsSpecifications;

			platform::gl::SetupOpenGLWindowing(r_Window, windowProperties, graphicsSpecification);
		}

	private:
		platform::Window& r_Window;
		ConfigData m_ConfigData;
	};
}