#pragma once
#include "platform/opengl_context.hpp"
#include "opengl/opengl_device.hpp"
#include "opengl/opengl_command_buffer.hpp"
#include "opengl/opengl_texture.hpp"

#include "platform/vulkan_context.hpp"
#include "graphics/vulkan/vulkan_device.hpp"
#include "graphics/vulkan/vulkan_command_buffer.hpp"
#include "graphics/vulkan/vulkan_gui_system.hpp"

namespace tur
{
#ifdef TUR_API_OPENGL
	using GraphicsDevice = tur::gl::GraphicsDeviceGL;
	using CommandBuffer = tur::gl::CommandBufferGL;

#elif TUR_API_VULKAN
	using GraphicsDevice = tur::vulkan::GraphicsDeviceVulkan;
	using CommandBuffer = tur::vulkan::CommandBufferVulkan;
	using GUISystem = tur::vulkan::VulkanGUI;
#endif

	inline void initialize_graphics_system(Window* window, const ConfigData& configData)
	{
#ifdef TUR_API_OPENGL
		gl::initialize_opengl_windowing(window, configData.windowProperties, configData.graphicsSpecification);

#elif TUR_API_VULKAN
		vulkan::initialize_vulkan_windowing(window, configData.windowProperties, configData.graphicsSpecification);
#endif
	}
}