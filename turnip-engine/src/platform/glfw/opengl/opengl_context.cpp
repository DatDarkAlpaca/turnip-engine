#include "pch.hpp"
#include <GLFW/glfw3.h>

#include "platform/glfw/window_glfw.hpp"
#include "graphics/graphics.hpp"

static void OpenGLCallback(unsigned, unsigned, unsigned, unsigned severity, int, const char* message, const void*)
{
	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH:
			TUR_LOG_CRITICAL(message);
			break;

		case GL_DEBUG_SEVERITY_MEDIUM:
			TUR_LOG_ERROR(message);
			break;

		case GL_DEBUG_SEVERITY_LOW:
			TUR_LOG_WARN(message);
			break;

		case GL_DEBUG_SEVERITY_NOTIFICATION:
			TUR_LOG_INFO(message);
			break;
	}

	TUR_LOG_INFO("[OpenGL][Unknown severity]: {}", message);
}

namespace tur
{
	void initialize_opengl_windowing(Window* window, const GraphicsSpecification& specification)
	{
#ifdef TUR_DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

#ifdef TUR_PLATFORM_MACOSX
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, specification.major);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, specification.minor);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
		glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		initialize_window(window);

		glfwMakeContextCurrent(window->window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			TUR_LOG_CRITICAL("Failed to load OpenGL functions.");

#ifdef TUR_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

		if (specification.major >= 4 && specification.minor >= 3)
		{
			glDebugMessageCallback(OpenGLCallback, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
		}
#endif
	}
}