#include "pch.h"
#include "SetupVulkan.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace tur::platform
{
	void SetupVulkan(Window& window)
	{
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		if (!glfwVulkanSupported())
			TUR_LOG_CRITICAL("Vulkan is not supported in this machine");

		window.Initialize(window.GetProperties());
	}

	std::vector<const char*> GetWindowingVulkanExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
		return extensions;
	}

	VkSurfaceKHR GetVulkanSurface(vk::Instance instance, Window* window)
	{
		VkSurfaceKHR surface;
		VkResult result = glfwCreateWindowSurface(instance, std::any_cast<GLFWwindow*>(window->GetHandle()), nullptr, &surface);

		if (result == VK_SUCCESS)
			return surface;

		TUR_LOG_CRITICAL("Failed to create window surface using GLFW and Vulkan");
	}
}