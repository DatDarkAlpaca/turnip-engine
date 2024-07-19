#pragma once
#include <vulkan/vulkan.hpp>
#include <vector>

#include "Renderpass.h"
#include "Frame.h"

namespace tur::vulkan
{
	struct Swapchain
	{
		vk::SwapchainKHR swapchain;
		std::vector<Frame> frames;
		uint32_t currentFrame = 0;

		vk::SurfaceFormatKHR surfaceFormat;
		vk::PresentModeKHR presentMode = vk::PresentModeKHR::eFifo;
		vk::Extent2D extent;

		RenderpassVulkan renderpass;
	};
}