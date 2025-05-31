#pragma once
#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>

#include "graphics/vulkan/objects/texture.hpp"
#include "graphics/vulkan/objects/swapchain.hpp"
#include "graphics/vulkan/objects/queue_list.hpp"
#include "graphics/vulkan/objects/frame_data.hpp"

namespace tur::vulkan
{
	struct VulkanState
	{
		// Instance:
		u32 apiVersion;
		vk::Instance instance;
		vk::DebugUtilsMessengerEXT debugMessenger;
		vk::DispatchLoaderDynamic DLDI;
		bool requiresDrawing;
		bool validationEnabled;

		// Surface:
		VkSurfaceKHR surface;

		// Physical Device:
		vk::PhysicalDevice physicalDevice;

		// Logical Device:
		vk::Device logicalDevice;
		QueueList queueList;

		// VMA:
		VmaAllocator vmaAllocator;

		// Swapchain:
		vk::SwapchainKHR swapchain;
		vk::SurfaceFormatKHR swapchainFormat;
		vk::Extent2D swapchainExtent;
		std::vector<vk::Image> swapChainImages;
		std::vector<vk::ImageView> swapChainImageViews;

		// Command Pool:
		vk::CommandPool commandPool;

		// Frame:
		FrameDataHolder frameDataHolder;
		Texture drawTexture;
	};
}