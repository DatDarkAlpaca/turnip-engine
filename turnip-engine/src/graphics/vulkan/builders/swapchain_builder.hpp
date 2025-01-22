#pragma once
#include <vulkan/vulkan.hpp>

#include "graphics/vulkan/objects/vulkan_state.hpp"
#include "graphics/vulkan/vulkan_textures.hpp"
#include "graphics/type/extent.hpp"

#include "core/config/config_data.hpp"
#include "common.hpp"

namespace tur::vulkan
{
	struct SwapchainRequirements
	{
		Extent2D extent;
		u32 imageCount = invalid_handle;

		// TODO: allow for specified swapchain format:
		// TextureFormat format = TextureFormat::B8G8R8A8_UNORM; 
		vk::PresentModeKHR presentMode = vk::PresentModeKHR::eMailbox;

		VkSwapchainKHR oldSwapchain = VK_NULL_HANDLE;
	};

	void initialize_swapchain(VulkanState& state, const SwapchainRequirements& requirements);
}