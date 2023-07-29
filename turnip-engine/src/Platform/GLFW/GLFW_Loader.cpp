#include "pch.h"
#include "GLFW_Loader.h"

static void GLAPIENTRY OpenGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	// Todo: improve.
	TUR_CORE_CRITICAL("GL Error: {}", message);
}

namespace tur
{
	void GLFW_Loader_OGL::PreInitialize()
	{
		if (!glfwInit())
		{
			TUR_CORE_ERROR("Failed to initialize GLFW.");
			return;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		TUR_WRAP_DEBUG(glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE));
	}

	void GLFW_Loader_OGL::PostInitialize(Window* window)
	{
		GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(window->GetWindow());
		glfwMakeContextCurrent(glfwWindow);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			TUR_CORE_ERROR("Failed to initialize GLAD.");
			return;
		}

		glDebugMessageCallback(OpenGLDebugCallback, nullptr);
	}
}
