#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Core/Defines.h"
#include "Graphics/BackendVersion.h"

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

	inline void InitializeOpenGL(GLFWwindow* window)
	{
		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			throw "Failed to initialize GLAD";
	}

	inline void SetWindowBufferSamples(uint32_t samples)
	{
		glfwWindowHint(GLFW_SAMPLES, samples);
	}

	inline void SwapBuffers(GLFWwindow* window)
	{
		glfwSwapBuffers(window);
	}
}