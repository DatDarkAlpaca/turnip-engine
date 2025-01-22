#include "pch.hpp"
#include "swapchain_builder.hpp"
#include "graphics/vulkan/objects/swapchain.hpp"

namespace tur::vulkan
{
	static vk::SurfaceFormatKHR choose_surface_format(const std::vector<vk::SurfaceFormatKHR>& formats)
	{
		for (const auto& surfaceFormat : formats) 
		{
			if (surfaceFormat.format == vk::Format::eB8G8R8A8Srgb && surfaceFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
				return surfaceFormat;
		}

		TUR_LOG_WARN("Failed to retrieve a surface format with the default format 'B8G8R8A8_SRGB' that supports SRGB");
		return formats[0];
	}
	
	static vk::PresentModeKHR choose_present_mode(const std::vector<vk::PresentModeKHR>& presentModes, vk::PresentModeKHR specifiedPresentMode)
	{
		for (const auto& presentMode : presentModes)
		{
			if (presentMode == specifiedPresentMode)
				return presentMode;
		}

		TUR_LOG_WARN("This device/surface does not support the specified present mode. Default: FIFO");
		return vk::PresentModeKHR::eFifo;
	}

	static vk::Extent2D choose_swapchain_extent(const vk::SurfaceCapabilitiesKHR& capabilities, const Extent2D& requestedExtent)
	{
		constexpr u32 invalid_unsigned = invalid_handle;

		if (capabilities.currentExtent.width != invalid_unsigned)
			return capabilities.currentExtent;

		VkExtent2D actualExtent = 
		{
			static_cast<u32>(requestedExtent.width),
			static_cast<u32>(requestedExtent.height)
		};

		actualExtent.width = std::clamp(
			actualExtent.width, 
			capabilities.minImageExtent.width, 
			capabilities.maxImageExtent.width
		);

		actualExtent.height = std::clamp(
			actualExtent.height, 
			capabilities.minImageExtent.height, 
			capabilities.maxImageExtent.height
		);

		return actualExtent;
	}

	void initialize_swapchain(VulkanState& state, const SwapchainRequirements& requirements)
	{
		auto swapchainCapabilities = fetch_swapchain_capabilities(state.physicalDevice, state.surface);
		
		vk::SurfaceFormatKHR surfaceFormat = choose_surface_format(swapchainCapabilities.availableSurfaceFormats);
		vk::PresentModeKHR presentMode = choose_present_mode(swapchainCapabilities.presentModes, requirements.presentMode);
		vk::Extent2D extent = choose_swapchain_extent(swapchainCapabilities.surfaceCapabilities, requirements.extent);
		
		vk::SwapchainCreateInfoKHR createInfo = {};
		{
			createInfo.flags = vk::SwapchainCreateFlagsKHR();
			createInfo.surface = state.surface;
			createInfo.imageFormat = surfaceFormat.format;

			if(requirements.imageCount == invalid_handle && requirements.imageCount > 0)
				createInfo.minImageCount = swapchainCapabilities.surfaceCapabilities.minImageCount + 1;
			else
				createInfo.minImageCount = requirements.imageCount;

			createInfo.imageExtent.width = extent.width;
			createInfo.imageExtent.height = extent.height;

			createInfo.preTransform = swapchainCapabilities.surfaceCapabilities.currentTransform;
			createInfo.presentMode = presentMode;

			createInfo.oldSwapchain = requirements.oldSwapchain;
		}

		// Queues:
		state.queueList;
		queue_family_index graphicsIndex = state.queueList.get_family_index(QueueUsage::GRAPHICS);
		queue_family_index presentIndex  = state.queueList.get_family_index(QueueUsage::PRESENT);

		if (graphicsIndex != presentIndex)
		{
			uint32_t indices[] = { graphicsIndex , presentIndex};
			createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = indices;
		}
		else 
			createInfo.imageSharingMode = vk::SharingMode::eExclusive;

		try
		{
			state.swapchain = state.logicalDevice.createSwapchainKHR(createInfo);
		}
		catch (const vk::SystemError& err)
		{
			TUR_LOG_ERROR("Failed to create swapchain: {}", err.what());
		}
		
		// Capabilities:
		state.swapchainCapabilities = swapchainCapabilities;

		// Retrieve Images:
		{
			state.swapChainImages = state.logicalDevice.getSwapchainImagesKHR(state.swapchain);
		}
	}
}