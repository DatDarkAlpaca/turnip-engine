#pragma once
#include "Common.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Platform/GLFW/Window_GLFW.h"

namespace tur::platform
{
	inline void ConfigureOpenGL(uint32_t versionMajor, uint32_t versionMinor)
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, versionMajor);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, versionMinor);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef TUR_PLATFORM_APPLE
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	}

	inline void InitializeOpenGL(Window* window)
	{
		glfwMakeContextCurrent(static_cast<GLFWwindow*>(window->GetHandle()));

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			TUR_LOG_CRITICAL("Failed to initialize a valid OpenGL context");
	}

	inline void SetWindowBufferSamples(uint32_t samples)
	{
		glfwWindowHint(GLFW_SAMPLES, samples);
	}

	inline void SwapBuffers(Window* window)
	{
		glfwSwapBuffers(static_cast<GLFWwindow*>(window->GetHandle()));
	}
}