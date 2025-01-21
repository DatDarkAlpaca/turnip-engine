#include "pch.hpp"
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#include "graphics/vulkan/vulkan_constants.hpp"
#include "platform/vulkan_context.hpp"
#include "platform/glfw/window_glfw.hpp"

namespace tur
{
	void initialize_vulkan_windowing(Window* window, const WindowProperties& properties, const GraphicsSpecification& specification)
	{
		if (!glfwVulkanSupported())
			TUR_LOG_CRITICAL("GLFW Vulkan is not supported in this machine");

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		
		glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
		glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		initialize_window(window, properties);
	}

	std::vector<const char*> get_windowing_vulkan_extensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
		return extensions;
	}

	VkSurfaceKHR get_vulkan_surface(vk::Instance instance, Window* window)
	{
		VkSurfaceKHR surface;
		VkResult result = glfwCreateWindowSurface(instance, std::any_cast<GLFWwindow*>(window->window), nullptr, &surface);

		if (result == VK_SUCCESS)
			return surface;

		TUR_LOG_CRITICAL("Failed to create window surface using GLFW and Vulkan");
		return nullptr;
	}

	std::vector<const char*> get_vulkan_extension_platform_surface_names()
	{
#ifdef TUR_PLATFORM_WIN32
		return { tur::vulkan::WIN32_SurfaceExtensionName };
#elif defined(TUR_PLATFORM_LINUX)
		return {
			tur::vulkan::XCB_SurfaceExtensionName,
			tur::vulkan::XLIB_SurfaceExtensionName,
			tur::vulkan::WAYLAND_SurfaceExtensionName
		};
#elif defined(TUR_PLATFORM_APPLE)
		return { METAL_SurfaceExtensionName };
#elif defined(TUR_PLATFORM_ANDROID)
		return { ANDROID_SurfaceExtensionName };
#endif
	}
}