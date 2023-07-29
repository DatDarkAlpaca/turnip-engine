#include "pch.h"
#include "GLFW_Loader.h"

#ifdef TUR_DEBUG

static const char* GetDebugGLSourceString(GLenum source)
{
	switch (source)
	{
		case GL_DEBUG_SOURCE_API:				return "API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		return "WINDOW_SYSTEM";
		case GL_DEBUG_SOURCE_SHADER_COMPILER:	return "SHADER_COMPILER";
		case GL_DEBUG_SOURCE_THIRD_PARTY:		return "THIRD_PARTY";
		case GL_DEBUG_SOURCE_APPLICATION:		return "SRC_APP";
		case GL_DEBUG_SOURCE_OTHER:				return "OTHER";
	}

	return "UNKNOWN";
}

static const char* GetDebugGLTypeString(GLenum type) 
{
	switch (type) 
	{
		case GL_DEBUG_TYPE_ERROR:					return "ERROR";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:		return "ERROR";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:		return "ERROR";
		case GL_DEBUG_TYPE_PORTABILITY:				return "ERROR";
		case GL_DEBUG_TYPE_PERFORMANCE:				return "ERROR";
		case GL_DEBUG_TYPE_OTHER:					return "ERROR";
		case GL_DEBUG_TYPE_MARKER:					return "ERROR";
	}

	return "UNKNOWN";
}

static void GLAPIENTRY OpenGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	const char* sourceString = GetDebugGLSourceString(source);
	const char* typeString = GetDebugGLTypeString(type);

	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH:
		{
			TUR_CORE_CRITICAL("[{}] GL:{} - {}", typeString, sourceString, message);
			TUR_BREAKPOINT();
		} break;

		case GL_DEBUG_SEVERITY_MEDIUM:
		{
			TUR_CORE_ERROR("[{}] GL:{} - {}", typeString, sourceString, message);
			TUR_BREAKPOINT();
		} break;

		case GL_DEBUG_SEVERITY_LOW:
		{
			TUR_CORE_WARN("[{}] GL:{} - {}", typeString, sourceString, message);
		} break;

		case GL_DEBUG_SEVERITY_NOTIFICATION:
		{
			TUR_CORE_INFO("[{}] GL:{} - {}", typeString, sourceString, message);
		} break;

		default:
		{
			TUR_CORE_DEBUG("[{}] GL:{} - {}", typeString, sourceString, message);
		} break;
	}
}

#endif

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

		// TODO: Requires(GL 4.3)
		TUR_WRAP_DEBUG (
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(OpenGLDebugCallback, nullptr);
		);
	}
}
