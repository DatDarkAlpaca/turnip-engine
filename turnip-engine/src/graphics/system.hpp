#pragma once
#include "platform/opengl_context.hpp"
#include "opengl/opengl_device.hpp"
#include "opengl/opengl_command_buffer.hpp"

#include "platform/vulkan_context.hpp"
#include "graphics/vulkan/vulkan_device.hpp"
#include "graphics/vulkan/vulkan_command_buffer.hpp"

namespace tur
{
#ifdef DAT_API_OPENGL
	using GraphicsDevice = tur::gl::GraphicsDeviceGL;
	using CommandBuffer = tur::gl::CommandBufferGL;

#elif DAT_API_VULKAN
	using GraphicsDevice = tur::vulkan::GraphicsDeviceVulkan;
	using CommandBuffer = tur::vulkan::CommandBufferVulkan;
#endif

	inline void initialize_graphics_system(Window* window, const WindowProperties& properties, const GraphicsSpecification& specification)
	{
#ifdef DAT_API_OPENGL
		gl::initialize_opengl_windowing(window, properties, specification);

#elif DAT_API_VULKAN
		vulkan::initialize_vulkan_windowing(window, properties, specification);
#endif
	}
}