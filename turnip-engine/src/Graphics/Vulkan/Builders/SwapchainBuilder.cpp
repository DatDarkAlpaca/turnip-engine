#include "pch.hpp"
#include "SwapchainBuilder.hpp"

namespace
{
	static std::vector<const char*> GetTransformBitNames(const vk::SurfaceTransformFlagsKHR& flags)
	{
		std::vector<const char*> results;

		if (flags & vk::SurfaceTransformFlagBitsKHR::eIdentity)
			results.push_back("Identity");

		if (flags & vk::SurfaceTransformFlagBitsKHR::eRotate90)
			results.push_back("Rotate90");

		if (flags & vk::SurfaceTransformFlagBitsKHR::eRotate180)
			results.push_back("Rotate180");

		if (flags & vk::SurfaceTransformFlagBitsKHR::eRotate270)
			results.push_back("Rotate270");

		if (flags & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirror)
			results.push_back("HorizontalMirror");

		if (flags & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate90)
			results.push_back("HorizontalMirrorRotate90");

		if (flags & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate180)
			results.push_back("HorizontalMirrorRotate180");

		if (flags & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate270)
			results.push_back("HorizontalMirrorRotate270");

		if (flags & vk::SurfaceTransformFlagBitsKHR::eInherit)
			results.push_back("Inherit");

		return results;
	}

	static std::vector<const char*> GetAlphaCompositeOperationBitNames(const vk::CompositeAlphaFlagsKHR& bits)
	{
		std::vector<const char*> results;

		if (bits & vk::CompositeAlphaFlagBitsKHR::eOpaque)
			results.push_back("Opaque");

		if (bits & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied)
			results.push_back("PreMultiplied");

		if (bits & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied)
			results.push_back("PostMultiplied");

		if (bits & vk::CompositeAlphaFlagBitsKHR::eInherit)
			results.push_back("Inherit");

		return results;
	}

	static std::vector<const char*> GetImageUsageBitNames(const vk::ImageUsageFlags& bits)
	{
		std::vector<const char*> results;

		if (bits & vk::ImageUsageFlagBits::eTransferSrc)
			results.push_back("TransferSrc");

		if (bits & vk::ImageUsageFlagBits::eTransferDst)
			results.push_back("TransferDst");

		if (bits & vk::ImageUsageFlagBits::eSampled)
			results.push_back("Sampled");

		if (bits & vk::ImageUsageFlagBits::eStorage)
			results.push_back("Storage");

		if (bits & vk::ImageUsageFlagBits::eColorAttachment)
			results.push_back("ColorAttachmen");

		if (bits & vk::ImageUsageFlagBits::eDepthStencilAttachment)
			results.push_back("DepthStencilAttachment");

		if (bits & vk::ImageUsageFlagBits::eTransientAttachment)
			results.push_back("TransientAttachment");

		if (bits & vk::ImageUsageFlagBits::eInputAttachment)
			results.push_back("InputAttachment");

		if (bits & vk::ImageUsageFlagBits::eFragmentDensityMapEXT)
			results.push_back("FragmentDensityMapEXT");

		if (bits & vk::ImageUsageFlagBits::eFragmentShadingRateAttachmentKHR)
			results.push_back("FragmentShadingRateAttachmentKHR");

		return results;
	}

	static const char* GetPresentModeName(vk::PresentModeKHR presentMode)
	{
		if (presentMode == vk::PresentModeKHR::eImmediate)
			return "Immediate";

		if (presentMode == vk::PresentModeKHR::eMailbox)
			return "Mailbox";

		if (presentMode == vk::PresentModeKHR::eFifo)
			return "FIFO";

		if (presentMode == vk::PresentModeKHR::eFifoRelaxed)
			return "FifoRelaxed";

		if (presentMode == vk::PresentModeKHR::eSharedDemandRefresh)
			return "SharedDemandRefresh";

		if (presentMode == vk::PresentModeKHR::eSharedContinuousRefresh)
			return "SharedContinuousRefresh";

		return "Unsupported";
	}
}

namespace tur::vulkan
{
	vk::SurfaceCapabilitiesKHR QuerySurfaceCapabilities(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface)
	{
		return device.getSurfaceCapabilitiesKHR(surface);
	}

	std::vector<vk::SurfaceFormatKHR> QuerySurfaceFormats(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface)
	{
		return device.getSurfaceFormatsKHR(surface);
	}

	std::vector<vk::PresentModeKHR> QuerySurfacePresentModes(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface)
	{
		return device.getSurfacePresentModesKHR(surface);
	}

	void DisplaySurfaceCapabilities(const vk::SurfaceCapabilitiesKHR& capabilities)
	{
		const auto& currentExtent = capabilities.currentExtent;
		const auto& minImageExtent = capabilities.minImageExtent;
		const auto& maxImageExtent = capabilities.maxImageExtent;

		TUR_LOG_INFO("Swapchain Surface Capabilities:");

		TUR_LOG_INFO("Image Count:");
		TUR_LOG_INFO("  * Minimum: {} | Maximum: {}", capabilities.minImageCount, capabilities.maxImageCount);
		TUR_LOG_INFO("  * Extent: (w: {}, h: {})", currentExtent.width, currentExtent.height);
		TUR_LOG_INFO("  * Minimum Extent: (w: {}, h: {})", minImageExtent.width, minImageExtent.height);
		TUR_LOG_INFO("  * Maximum Extent: (w: {}, h: {})", maxImageExtent.width, maxImageExtent.height);
		TUR_LOG_INFO("  * Maximum Image Array Layers: {}x", capabilities.maxImageArrayLayers);

		TUR_LOG_INFO("Supported Transforms:");
		for (const auto& transformBit : GetTransformBitNames(capabilities.supportedTransforms))
			TUR_LOG_INFO("  * {}", transformBit);

		TUR_LOG_INFO("Current Transform:");
		for (const auto& transformBit : GetTransformBitNames(capabilities.currentTransform))
			TUR_LOG_INFO("  * {}", transformBit);

		TUR_LOG_INFO("Supported Alpha Operations:");
		for (const auto& alphaOperationBit : GetAlphaCompositeOperationBitNames(capabilities.supportedCompositeAlpha))
			TUR_LOG_INFO("  * {}", alphaOperationBit);

		TUR_LOG_INFO("Supported Image Usages:");
		for (const auto& imageUsageBit : GetImageUsageBitNames(capabilities.supportedUsageFlags))
			TUR_LOG_INFO("  * {}", imageUsageBit);
	}

	void DisplaySurfaceFormats(const std::vector<vk::SurfaceFormatKHR>& formats)
	{
		TUR_LOG_INFO("Supported Pixel Formats:");
		for (vk::SurfaceFormatKHR supportedFormat : formats)
			TUR_LOG_INFO("  * {} | {}", vk::to_string(supportedFormat.format), vk::to_string(supportedFormat.colorSpace));
	}

	void DisplayPresentModes(const std::vector<vk::PresentModeKHR>& presentModes)
	{
		TUR_LOG_INFO("Supported Present Modes:");
		for (vk::PresentModeKHR presentMode : presentModes)
			TUR_LOG_INFO("  * {}", GetPresentModeName(presentMode));
	}
}

namespace tur::vulkan
{
	SwapchainBuilder::SwapchainBuilder(const vk::SurfaceKHR& surface, const vk::PhysicalDevice& physicalDevice, const vk::Device& device, const QueueCluster& queues)
		: m_Surface(surface)
		, m_PhysicalDevice(physicalDevice)
		, m_LogicalDevice(device)
		, m_Queues(queues)
	{
		m_ArgumentsSet = true;
		Prepare();
	}

	SwapchainBuilder& SwapchainBuilder::Prepare()
	{
		if (!m_ArgumentsSet)
			TUR_LOG_CRITICAL("You must set the arguments before creating a swapchain. Use SetArguments()");

		m_Supported.capabilities = QuerySurfaceCapabilities(m_PhysicalDevice, m_Surface);
		m_Supported.surfaceFormats = QuerySurfaceFormats(m_PhysicalDevice, m_Surface);
		m_Supported.presentModes = QuerySurfacePresentModes(m_PhysicalDevice, m_Surface);
		m_Prepared = true;

		return *this;
	}

	std::optional<SwapchainObject> SwapchainBuilder::Create()
	{
		if (!m_Prepared)
			TUR_LOG_CRITICAL("The swapchain supported features haven't been cached. Call Prepare() before (re)creating a swapchain");

		if (!m_SurfaceFormatSet)
		{
			TUR_LOG_WARN("No surface format has been found for the current swapchain. Creating a default one.");
			SetSurfaceFormat();
		}

		if (!m_ImageCountSet)
		{
			TUR_LOG_WARN("No image count has been specificed. Using the minimum amount of images.");
			SetMinimumImageCount(0);
		}

		if (!m_ExtentSet)
		{
			TUR_LOG_WARN("No extent has been specificed. Using the minimum extent possible.");
			SetExtent({ 0, 0 });
		}

		auto& info = m_Information;

		vk::SwapchainCreateInfoKHR createInfo = vk::SwapchainCreateInfoKHR(
			vk::SwapchainCreateFlagsKHR(), m_Surface,
			info.minImageCount, info.surfaceFormat.format, info.surfaceFormat.colorSpace,
			info.extent, info.imageArrayLayers,
			info.imageUsage
		);

		createInfo.preTransform = info.preTransform;
		createInfo.compositeAlpha = info.compositeAlpha;
		createInfo.presentMode = info.presentMode;
		createInfo.clipped = info.clipped;
		createInfo.oldSwapchain = info.oldSwapchain;

		PrepareQueueInformation(createInfo);

		SwapchainObject swapchainObject;
		try
		{
			swapchainObject.swapchain = m_LogicalDevice.createSwapchainKHR(createInfo);
		}
		catch (const vk::SystemError& err)
		{
			TUR_LOG_ERROR("Failed to create swapchain: {}", err.what());
			return std::nullopt;
		}

		// Information:
		swapchainObject.surfaceFormat = m_Information.surfaceFormat;
		swapchainObject.presentMode = m_Information.presentMode;
		swapchainObject.extent = m_Information.extent;

		return swapchainObject;
	}

	SwapchainBuilder& SwapchainBuilder::SetArguments(vk::SurfaceKHR surface, vk::PhysicalDevice physicalDevice, vk::Device device, const QueueCluster& queues)
	{
		m_Surface = surface;
		m_PhysicalDevice = physicalDevice;
		m_LogicalDevice = device;
		m_Queues = queues;

		m_ArgumentsSet = true;
		return *this;
	}

	vk::SurfaceFormatKHR SwapchainBuilder::DefaultSurfaceFormatSelector(const std::vector<vk::SurfaceFormatKHR>& formats)
	{
		for (const vk::SurfaceFormatKHR& format : formats)
		{
			if (format.format == vk::Format::eB8G8R8A8Unorm &&
				format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
				return format;
		}

		return formats[0];
	}
	
	SwapchainBuilder& SwapchainBuilder::SetSurfaceFormat(const vk::SurfaceFormatKHR& surfaceFormat)
	{
		m_Information.surfaceFormat = surfaceFormat;
		m_SurfaceFormatSet = true;
		return *this;
	}
	
	SwapchainBuilder& SwapchainBuilder::SetSurfaceFormat(const SwapchainFormatSelector& formatSelector)
	{
#ifdef TUR_DEBUG
		if (!m_Prepared)
			TUR_LOG_CRITICAL("Please call Prepare() before setting a custom surface format selector");
#endif

		m_Information.surfaceFormat = formatSelector(m_Supported.surfaceFormats);
		m_SurfaceFormatSet = true;
		return *this;
	}

	vk::PresentModeKHR SwapchainBuilder::DefaultPresentModeSelector(const std::vector<vk::PresentModeKHR>& presentModes)
	{
		for (const vk::PresentModeKHR& presentMode : presentModes)
		{
			if (presentMode == vk::PresentModeKHR::eMailbox)
				return presentMode;
		}

		return vk::PresentModeKHR::eFifo;
	}

	SwapchainBuilder& SwapchainBuilder::SetPresentMode(const vk::PresentModeKHR& presentMode)
	{
		m_Information.presentMode = presentMode;
		return *this;
	}

	SwapchainBuilder& SwapchainBuilder::SetPresentMode(const PresentModeSelector& presentModeSelector)
	{
#ifdef TUR_DEBUG
		if (!m_Prepared)
			TUR_LOG_CRITICAL("Please call Prepare() before setting a custom present mode selector");
#endif

		m_Information.presentMode = presentModeSelector(m_Supported.presentModes);
		return *this;
	}

	SwapchainBuilder& SwapchainBuilder::SetOldSwapchain(const SwapchainObject& swapchain)
	{
		m_Information.oldSwapchain = swapchain.swapchain;
		return *this;
	}

	SwapchainBuilder& SwapchainBuilder::SetOldSwapchain(const vk::SwapchainKHR& swapchain)
	{
		m_Information.oldSwapchain = swapchain;
		return *this;
	}

	SwapchainBuilder& SwapchainBuilder::SetExtent(vk::Extent2D extent)
	{
#ifdef TUR_DEBUG
		if (!m_Prepared)
			TUR_LOG_CRITICAL("Please call Prepare() before setting a swapchain extent");
#endif

		m_Information.extent = extent;

		m_Information.extent.width = std::clamp(
			m_Information.extent.width,
			m_Supported.capabilities.minImageExtent.width,
			m_Supported.capabilities.maxImageExtent.width);

		m_Information.extent.height = std::clamp(
			m_Information.extent.height,
			m_Supported.capabilities.minImageExtent.height,
			m_Supported.capabilities.maxImageExtent.height);

		m_ExtentSet = true;

		return *this;
	}

	SwapchainBuilder& SwapchainBuilder::SetDefaultSharingMode()
	{
		m_SharingModeOverride = false;
		return *this;
	}

	SwapchainBuilder& SwapchainBuilder::SetSharingMode(vk::SharingMode sharingMode)
	{
		m_Information.sharingMode = sharingMode;
		m_SharingModeOverride = true;
		return *this;
	}

	SwapchainBuilder& SwapchainBuilder::SetImageUsage(vk::ImageUsageFlags imageUsage)
	{
#if TUR_DEBUG
		auto& supportedImageUsageFlags = m_Supported.capabilities.supportedUsageFlags;
		auto query = supportedImageUsageFlags & imageUsage;
		if (!query)
		{
			TUR_LOG_ERROR("This device does not support one of the following usage image flags:");
			for (const auto& imageUsage : GetImageUsageBitNames(imageUsage))
				TUR_LOG_ERROR("  * {}", imageUsage);
		}
#endif

		m_Information.imageUsage = imageUsage;
		return *this;
	}

	SwapchainBuilder& SwapchainBuilder::SetPreTransform(vk::SurfaceTransformFlagBitsKHR preTransform)
	{
#if TUR_DEBUG
		auto& supportedPreTransforms = m_Supported.capabilities.supportedTransforms;
		auto query = supportedPreTransforms & preTransform;
		if (!query)
		{
			TUR_LOG_ERROR("This device does not support one of the following swapchain pre-transform flags:");
			for (const auto& transformName : GetTransformBitNames(preTransform))
				TUR_LOG_ERROR("  * {}", transformName);
		}
#endif

		m_Information.preTransform = preTransform;
		return *this;
	}

	SwapchainBuilder& SwapchainBuilder::SetCompositeAlpha(vk::CompositeAlphaFlagBitsKHR compositeAlpha)
	{
#if TUR_DEBUG
		auto& supportedCompositeAlphaFlags = m_Supported.capabilities.supportedCompositeAlpha;
		auto query = supportedCompositeAlphaFlags & compositeAlpha;
		if (!query)
		{
			TUR_LOG_ERROR("This device does not support one of the following composite alpha flags:");
			for (const auto& alphaFlag : GetAlphaCompositeOperationBitNames(compositeAlpha))
				TUR_LOG_ERROR("  * {}", alphaFlag);
		}
#endif

		m_Information.compositeAlpha = compositeAlpha;
		return *this;
	}

	SwapchainBuilder& SwapchainBuilder::SetMinimumImageCount(uint32_t minimumImageCount)
	{
		m_Information.minImageCount = std::clamp(
			minimumImageCount,
			m_Supported.capabilities.minImageCount,
			m_Supported.capabilities.maxImageCount
		);

		m_ImageCountSet = true;

		return *this;
	}
	
	SwapchainBuilder& SwapchainBuilder::SetImageArrayLayers(uint32_t imageArrayLayers)
	{
		m_Information.imageArrayLayers = imageArrayLayers;
		return *this;
	}
	
	SwapchainBuilder& SwapchainBuilder::SetClipped(bool clipped)
	{
		m_Information.clipped = clipped;
		return *this;
	}

	void SwapchainBuilder::PrepareQueueInformation(vk::SwapchainCreateInfoKHR& createInfo)
	{
		std::vector<uint32_t> queueIndices;
		queueIndices.reserve(m_Queues.queues.size());

		for (const auto& [index, queue, operation] : m_Queues.queues)
			queueIndices.push_back(index);

		if (queueIndices.size() > 1)
		{
			createInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueIndices.size());
			createInfo.pQueueFamilyIndices = queueIndices.data();
		}
		else
			createInfo.imageSharingMode = vk::SharingMode::eExclusive;

		// Final Sharing Mode:
		if (m_SharingModeOverride)
			createInfo.imageSharingMode = m_Information.sharingMode;
		else
			m_Information.sharingMode = createInfo.imageSharingMode;
	}
}