#pragma once
#include <vulkan/vulkan.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "Core/Defines.h"
#include "Core/Logger/Logger.h"

namespace tur::platform
{
	inline void ConfigureVulkan()
	{
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	}

	inline std::vector<const char*> GetWindowingVulkanExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
		return extensions;
	}

	inline VkSurfaceKHR GetVulkanSurface(vk::Instance instance, void* window)
	{
		VkSurfaceKHR surface;
		VkResult result = glfwCreateWindowSurface(instance, static_cast<GLFWwindow*>(window), nullptr, &surface);

		if (result != VK_SUCCESS)
			TUR_LOG_ERROR("Failed to create window surface using GLFW and Vulkan");

		return surface;
	}
}