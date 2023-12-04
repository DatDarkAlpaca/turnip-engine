#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include "Core/Defines.h"

namespace tur::platform
{
	inline void ConfigureVulkan()
	{
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	}

	inline std::vector<const char*> GetGLFWVulkanExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
		return extensions;
	}
}