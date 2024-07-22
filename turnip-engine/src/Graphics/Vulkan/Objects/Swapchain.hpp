#pragma once
#include <vulkan/vulkan.hpp>
#include <vector>

#include "Renderpass.hpp"
#include "Frame.hpp"

namespace tur::vulkan
{
	struct SwapchainObject
	{
		vk::SwapchainKHR swapchain;
		std::vector<Frame> frames;
		uint32_t currentFrame = 0;

		vk::SurfaceFormatKHR surfaceFormat;
		vk::PresentModeKHR presentMode = vk::PresentModeKHR::eFifo;
		vk::Extent2D extent;

		RenderpassObject renderpassObject;
	};
}