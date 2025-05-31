#pragma once
#include <vector>
#include <vulkan/vulkan.hpp>

#include "graphics/graphics_specification.hpp"
#include "platform/window.hpp"

namespace tur::vulkan
{
	class GraphicsDeviceVulkan;

	extern void initialize_vulkan_windowing(Window* window, const WindowProperties& properties, const GraphicsSpecification& specification);

	extern void initialize_vulkan_gui(vulkan::GraphicsDeviceVulkan* device);

	extern void begin_vulkan_frame();

	extern void render_vulkan_frame(vk::CommandBuffer commandBuffer);

	extern void end_vulkan_frame();

	extern std::vector<const char*> get_windowing_vulkan_extensions();

	extern VkSurfaceKHR get_vulkan_surface(vk::Instance instance, Window* window);

	extern std::vector<const char*> get_vulkan_extension_platform_surface_names();
}