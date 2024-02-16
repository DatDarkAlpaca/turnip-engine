#pragma once
#include <vulkan/vulkan.hpp>
#include <vector>

#include "Frame.h"

namespace tur::vulkan
{
	struct Swapchain
	{
		vk::SwapchainKHR swapchain;
		std::vector<Frame> frames;

		vk::SurfaceFormatKHR surfaceFormat;
		vk::PresentModeKHR presentMode = vk::PresentModeKHR::eFifo;
		vk::Extent2D extent;
	};
}