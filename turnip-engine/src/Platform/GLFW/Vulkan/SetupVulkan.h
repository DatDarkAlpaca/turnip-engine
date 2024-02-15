#pragma once
#include "Core/Defines.h"
#include "Core/Logger/Logger.h"

#include <vulkan/vulkan.hpp>
#include "Platform/GLFW/Window_GLFW.h"

namespace tur::platform
{
	void SetupVulkan();

	std::vector<const char*> GetWindowingVulkanExtensions();

	VkSurfaceKHR GetVulkanSurface(vk::Instance instance, Window* window);
}