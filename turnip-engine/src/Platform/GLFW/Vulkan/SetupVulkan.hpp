#pragma once
#include "Core/Defines.hpp"
#include "Core/Logger/Logger.hpp"

#include <vulkan/vulkan.hpp>
#include "Platform/GLFW/Window_GLFW.hpp"

namespace tur::platform::vulkan
{
	void SetupVulkanWindowing(Window& window, const WindowProperties&);

	std::vector<const char*> GetWindowingVulkanExtensions();

	VkSurfaceKHR GetVulkanSurface(vk::Instance instance, Window* window);
}