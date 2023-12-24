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
		m_Properties.major = properties.major;
		m_Properties.minor = properties.minor;

		if (m_Properties.major == 0xFFFFFFFF)
			m_Properties.major = DefaultVersionMajor;
		if (m_Properties.minor == 0xFFFFFFFF)
			m_Properties.minor = DefaultVersionMinor;

		platform::ConfigureOpenGL(m_Properties.major, m_Properties.minor);
	}

	void BackendOpenGL::FinishSetup(tur_unique<Window>& window)
	{
		platform::InitializeOpenGL(window.get());
		m_Window = window.get();

		DisplayOpenGLInformation();
	}

	void BackendOpenGL::Present()
	{
		platform::SwapBuffers(m_Window);
	}

	tur_unique<IShader> BackendOpenGL::CreateShader(const ShaderDescriptor& descriptor)
	{
		return tur::MakeUnique<ShaderOpenGL>(descriptor);
	}

	tur_unique<IPipeline> BackendOpenGL::CreatePipeline(const PipelineDescriptor& descriptor)
	{
		return tur::MakeUnique<PipelineOpenGL>(descriptor, m_Window);
	}
}