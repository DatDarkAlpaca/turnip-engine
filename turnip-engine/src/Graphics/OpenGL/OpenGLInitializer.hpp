#pragma once
#include "Core/Config/ConfigSystem.hpp"
#include "Platform/Platform.hpp"

namespace tur::gl
{
	class OpenGLInitializer
	{
	public:
		explicit OpenGLInitializer(const ConfigSystem& configSystem, platform::Window& window)
			: r_Window(window)
			, m_ConfigSystem(configSystem)
		{

		}

	public:
		void Initialize()
		{
			auto& windowProperties = m_ConfigSystem.GetWindowProperties();
			auto& graphicsSpecification = m_ConfigSystem.GetGraphicsSpecification();

			platform::gl::SetupOpenGLWindowing(r_Window, windowProperties, graphicsSpecification);
		}

	private:
		platform::Window& r_Window;
		ConfigSystem m_ConfigSystem;
	};
}