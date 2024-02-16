#pragma once
#include <optional>
#include <vulkan/vulkan.hpp>

#include "Common.h"
#include "Core/Logger/Logger.h"

#include "Graphics/Vulkan/Objects/Objects.h"

namespace tur::vulkan
{
	vk::SurfaceCapabilitiesKHR QuerySurfaceCapabilities(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface);

	std::vector<vk::SurfaceFormatKHR> QuerySurfaceFormats(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface);

	std::vector<vk::PresentModeKHR> QuerySurfacePresentModes(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface);

	void DisplaySurfaceCapabilities(const vk::SurfaceCapabilitiesKHR& capabilities);

	void DisplaySurfaceFormats(const std::vector<vk::SurfaceFormatKHR>& formats);

	void DisplayPresentModes(const std::vector<vk::PresentModeKHR>& presentModes);
}

namespace tur::vulkan
{
	class SwapchainBuilder
	{
		using SwapchainFormatSelector = std::function<vk::SurfaceFormatKHR(const std::vector<vk::SurfaceFormatKHR>)>;
		using PresentModeSelector = std::function<vk::PresentModeKHR(const std::vector<vk::PresentModeKHR>)>;

	public:
		SwapchainBuilder(
			const vk::SurfaceKHR& surface,
			const vk::PhysicalDevice& physicalDevice,
			const vk::Device& device,
			const QueueCluster& queues
		);

		SwapchainBuilder() = default;

	public:
		SwapchainBuilder& Prepare();

		std::optional<Swapchain> Create();

	public:
		SwapchainBuilder& SetArguments(vk::SurfaceKHR surface, vk::PhysicalDevice physicalDevice, vk::Device device, const QueueCluster& queues);

	public:
		static vk::SurfaceFormatKHR DefaultSurfaceFormatSelector(const std::vector<vk::SurfaceFormatKHR>& formats);
		SwapchainBuilder& SetSurfaceFormat(const vk::SurfaceFormatKHR& surfaceFormat);
		SwapchainBuilder& SetSurfaceFormat(const SwapchainFormatSelector& formatSelector = SwapchainBuilder::DefaultSurfaceFormatSelector);

		static vk::PresentModeKHR DefaultPresentModeSelector(const std::vector<vk::PresentModeKHR>& presentModes);
		SwapchainBuilder& SetPresentMode(const vk::PresentModeKHR& presentMode);
		SwapchainBuilder& SetPresentMode(const PresentModeSelector& presentModeSelector = SwapchainBuilder::DefaultPresentModeSelector);

		SwapchainBuilder& SetOldSwapchain(const Swapchain& swapchain);
		SwapchainBuilder& SetOldSwapchain(const vk::SwapchainKHR& swapchain);

		SwapchainBuilder& SetExtent(vk::Extent2D extent);

		SwapchainBuilder& SetDefaultSharingMode();
		SwapchainBuilder& SetSharingMode(vk::SharingMode sharingMode);

		SwapchainBuilder& SetImageUsage(vk::ImageUsageFlags imageUsage);

		SwapchainBuilder& SetPreTransform(vk::SurfaceTransformFlagBitsKHR preTransform);

		SwapchainBuilder& SetCompositeAlpha(vk::CompositeAlphaFlagBitsKHR compositeAlpha);

		SwapchainBuilder& SetMinimumImageCount(uint32_t minimumImageCount);
		SwapchainBuilder& SetImageArrayLayers(uint32_t imageArrayLayers);

		SwapchainBuilder& SetClipped(bool clipped);

	private:
		void PrepareQueueInformation(vk::SwapchainCreateInfoKHR& createInfo);

	private:
		vk::SurfaceKHR m_Surface;
		vk::PhysicalDevice m_PhysicalDevice;
		vk::Device m_LogicalDevice;
		QueueCluster m_Queues;
		
		struct SupportedFeatures
		{
			vk::SurfaceCapabilitiesKHR capabilities;
			std::vector<vk::SurfaceFormatKHR> surfaceFormats;
			std::vector<vk::PresentModeKHR> presentModes;
		} m_Supported;
		
		struct SwapchainInformation
		{
			vk::SurfaceFormatKHR surfaceFormat;
			vk::PresentModeKHR presentMode = vk::PresentModeKHR::eFifo;

			vk::SwapchainKHR oldSwapchain = nullptr;
			vk::Extent2D extent = vk::Extent2D(256, 256);
			vk::SharingMode sharingMode = vk::SharingMode::eConcurrent;

			vk::ImageUsageFlags imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
			vk::SurfaceTransformFlagBitsKHR preTransform = vk::SurfaceTransformFlagBitsKHR::eIdentity;
			vk::CompositeAlphaFlagBitsKHR compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
						
			uint32_t minImageCount = 0;
			uint32_t imageArrayLayers = 1;

			bool clipped = true;
		} m_Information;

		bool m_ArgumentsSet = false, m_Prepared = false;
		bool m_SurfaceFormatSet = false;
		bool m_SharingModeOverride = false;
		bool m_ImageCountSet = false;
		bool m_ExtentSet = false;
	};
}