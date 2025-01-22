#pragma once
#include <vulkan/vulkan.hpp>

namespace tur::vulkan
{
	struct SwapchainCapabilities
	{
		vk::SurfaceCapabilitiesKHR surfaceCapabilities;
		std::vector<vk::SurfaceFormatKHR> availableSurfaceFormats;
		std::vector<vk::PresentModeKHR> presentModes;
	};

	inline SwapchainCapabilities fetch_swapchain_capabilities(vk::PhysicalDevice device, VkSurfaceKHR surface)
	{
		SwapchainCapabilities capabilities;
		capabilities.surfaceCapabilities = device.getSurfaceCapabilitiesKHR(surface);

		auto surfaceFormats = device.getSurfaceFormatsKHR(surface);

		capabilities.availableSurfaceFormats.reserve(surfaceFormats.size());
		for (const auto& surfaceFormat : surfaceFormats)
			capabilities.availableSurfaceFormats.push_back(surfaceFormat);

		auto presentModes = device.getSurfacePresentModesKHR(surface);
		capabilities.presentModes.reserve(presentModes.size());
		for (const auto& presentMode : presentModes)
			capabilities.presentModes.push_back(presentMode);

		return capabilities;
	}
}