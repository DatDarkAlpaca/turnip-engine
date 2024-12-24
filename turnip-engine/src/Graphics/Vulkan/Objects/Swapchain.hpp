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

		vk::SurfaceFormatKHR surfaceFormat;
		vk::PresentModeKHR presentMode = vk::PresentModeKHR::eFifo;
		vk::Extent2D extent;
	};
}