#include "pch.h"
#include "BackendOpenGL.h"
#include "Platform/Platform.h"

#include "ShaderOpenGL.h"
#include "PipelineOpenGL.h"

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
		platform::ConfigureOpenGL(m_Properties.version.major, m_Properties.version.minor);

		if(!window)
			window = tur::MakeUnique<Window>();
		
		RecreateWindow(window.get());
		
		m_Window = window.get();

		platform::InitializeOpenGL(static_cast<GLFWwindow*>(window->GetHandle()));
	
		DisplayOpenGLInformation();
	}

	void BackendOpenGL::Present()
	{
		platform::SwapBuffers(static_cast<GLFWwindow*>(m_Window->GetHandle()));
	}

	tur_unique<Shader> BackendOpenGL::CreateShader(const ShaderDescriptor& descriptor)
	{
		return tur::MakeUnique<ShaderOpenGL>(descriptor);
	}

	tur_unique<Pipeline> BackendOpenGL::CreatePipeline(const PipelineDescriptor& descriptor)
	{
		return tur::MakeUnique<PipelineOpenGL>(descriptor, m_Window);
	}
}