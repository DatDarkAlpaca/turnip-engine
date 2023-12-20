#pragma once

namespace tur
{
	struct SwapchainFrame
	{
		vk::Image image;
		vk::ImageView view;
		vk::Framebuffer framebuffer;
		vk::CommandBuffer commandBuffer;
	};
}