#pragma once
#include "graphics_specification.hpp"
#include "objects/swapbuffer.hpp"
#include "objects/buffer.hpp"
#include "objects/texture.hpp"
#include "camera.hpp"

#include "opengl/opengl_device.hpp"
#include "opengl/opengl_command_buffer.hpp"

namespace tur
{
#ifdef DAT_API_OPENGL
	using GraphicsDevice = tur::gl::GraphicsDeviceGL;
	using CommandBuffer = tur::gl::CommandBufferGL;

#elif DAT_API_VULKAN
	using GraphicsDevice = vulkan::GraphicsDeviceVulkan;
	using CommandBuffer = vulkan::CommandBufferVulkan;

#endif
}