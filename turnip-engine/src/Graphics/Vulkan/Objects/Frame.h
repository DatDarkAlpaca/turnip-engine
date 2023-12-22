#pragma once
#include <vulkan/vulkan.hpp>

namespace tur::vulkan
{
	struct Frame
	{
		vk::Image image;
		vk::ImageView view;
		vk::Framebuffer framebuffer;
		vk::CommandBuffer commandBuffer;
	};
}