#pragma once
#include <vector>
#include <vulkan/vulkan.hpp>

#include "graphics/graphics_specification.hpp"
#include "platform/window.hpp"

namespace tur
{
	extern void initialize_vulkan_windowing(Window* window, const WindowProperties& properties, const GraphicsSpecification& specification);

	extern std::vector<const char*> get_windowing_vulkan_extensions();

	extern VkSurfaceKHR get_vulkan_surface(vk::Instance instance, Window* window);

	extern std::vector<const char*> get_vulkan_extension_platform_surface_names();
}