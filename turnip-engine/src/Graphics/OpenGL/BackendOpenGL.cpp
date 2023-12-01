#include "pch.h"
#include "BackendOpenGL.h"

#include "Platform/Platform.h"

namespace tur
{
	static void DisplayOpenGLInformation()
	{
		TUR_LOG_DEBUG("Selected Graphics Backend: {}", reinterpret_cast<char const*>(glGetString(GL_VERSION)));
		TUR_LOG_DEBUG(" * Vendor: {}", reinterpret_cast<char const*>(glGetString(GL_VENDOR)));
		TUR_LOG_DEBUG(" * Renderer: {}", reinterpret_cast<char const*>(glGetString(GL_RENDERER)));
 	}
}

namespace tur
{
	void BackendOpenGL::Initialize(const BackendProperties& properties)
	{
		uint32_t versionMajor = properties.version.major;
		uint32_t versionMinor = properties.version.minor;

		if (versionMajor == 0xFFFFFFFF)
			versionMajor = DefaultVersionMajor;
		if (versionMinor == 0xFFFFFFFF)
			versionMinor = DefaultVersionMinor;

		platform::ConfigureOpenGL(versionMajor, versionMinor);

		WindowProperties windowProperties = r_Window.get() ? r_Window->GetProperties() : WindowProperties{};
		r_Window = tur::MakeUnique<Window>();
		r_Window->Initialize(windowProperties);

		platform::InitializeOpenGL(static_cast<GLFWwindow*>(r_Window->GetHandle()));
	
		DisplayOpenGLInformation();
	}

	void BackendOpenGL::Present()
	{
		platform::SwapBuffers(static_cast<GLFWwindow*>(r_Window->GetHandle()));
	}
}