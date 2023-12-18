#include "pch.h"
#include "BackendOpenGL.h"
#include "Platform/Platform.h"

#include "ShaderOpenGL.h"

namespace tur
{
	static void DisplayOpenGLInformation()
	{
		TUR_LOG_DEBUG("Selected Graphics Backend: OpenGL {}", reinterpret_cast<char const*>(glGetString(GL_VERSION)));
		TUR_LOG_DEBUG(" * Vendor: {}", reinterpret_cast<char const*>(glGetString(GL_VENDOR)));
		TUR_LOG_DEBUG(" * Renderer: {}", reinterpret_cast<char const*>(glGetString(GL_RENDERER)));
 	}
}

namespace tur
{
	BackendOpenGL::BackendOpenGL(const BackendProperties& properties)
		: m_Properties(properties)
	{
		uint32_t versionMajor = properties.version.major;
		uint32_t versionMinor = properties.version.minor;

		if (versionMajor == 0xFFFFFFFF)
			m_Properties.version.major = DefaultVersionMajor;
		if (versionMinor == 0xFFFFFFFF)
			m_Properties.version.minor = DefaultVersionMinor;
	}

	void BackendOpenGL::InitializeWindow(tur_unique<Window>& window)
	{
		m_Window = window.get();

		platform::ConfigureOpenGL(m_Properties.version.major, m_Properties.version.minor);

		WindowProperties windowProperties = window.get() ? window->GetProperties() : WindowProperties{};
		window = tur::MakeUnique<Window>();
		window->Initialize(windowProperties);

		platform::InitializeOpenGL(static_cast<GLFWwindow*>(window->GetHandle()));
	
		DisplayOpenGLInformation();
	}

	void BackendOpenGL::Present()
	{
		platform::SwapBuffers(static_cast<GLFWwindow*>(m_Window->GetHandle()));
	}

	Shader* BackendOpenGL::CreateShader(const ShaderDescriptor& descriptor)
	{
		return new ShaderOpenGL(descriptor);
	}
}