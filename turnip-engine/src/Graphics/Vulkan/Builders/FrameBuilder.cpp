#include "pch.hpp"
#include "FrameBuilder.hpp"

namespace tur::vulkan
{
	FrameBuilder::FrameBuilder()
	{
		m_ComponentSwizzleFlags.fill(vk::ComponentSwizzle::eIdentity);
	}

	Frames FrameBuilder::Build(vk::Device device, SwapchainObject swapchain)
	{
		auto swapchainImages = device.getSwapchainImagesKHR(swapchain.swapchain);

		Frames frames;
		for (size_t i = 0; i < swapchainImages.size(); ++i)
		{
			Frame frameData;

			// Create Image View:
			vk::ImageViewCreateInfo createInfo = {};
			createInfo.image = swapchainImages[i];
			createInfo.viewType = m_ViewType;
			createInfo.format = swapchain.surfaceFormat.format;
			createInfo.components.r = m_ComponentSwizzleFlags[0];
			createInfo.components.g = m_ComponentSwizzleFlags[1];
			createInfo.components.b = m_ComponentSwizzleFlags[2];
			createInfo.components.a = m_ComponentSwizzleFlags[3];
			createInfo.subresourceRange.aspectMask = m_ImageAspect;
			createInfo.subresourceRange.baseMipLevel = m_BaseMipLevel;
			createInfo.subresourceRange.levelCount = m_LevelCount;
			createInfo.subresourceRange.baseArrayLayer = m_BaseArrayLayer;
			createInfo.subresourceRange.layerCount = m_LayerCount;

			frameData.image = swapchainImages[i];
			frameData.view = device.createImageView(createInfo);

			frames.Add(frameData);
		}

		return frames;
	}

	FrameBuilder& FrameBuilder::SetViewType(vk::ImageViewType viewType)
	{
		m_ViewType = viewType;
		return *this;
	}

	FrameBuilder& FrameBuilder::SetComponentSwizzle(const std::array<vk::ComponentSwizzle, 4>& swizzleFlags)
	{
		m_ComponentSwizzleFlags = swizzleFlags;
		return *this;
	}

	FrameBuilder& FrameBuilder::SetComponentSwizzle(vk::ComponentSwizzle r, vk::ComponentSwizzle g, vk::ComponentSwizzle b, vk::ComponentSwizzle a)
	{
		m_ComponentSwizzleFlags = { r,g,b,a };
		return *this;
	}

	FrameBuilder& FrameBuilder::SetAspectMask(vk::ImageAspectFlagBits imageAspect)
	{
		m_ImageAspect = imageAspect;
		return *this;
	}

	FrameBuilder& FrameBuilder::SetBaseMipLevel(uint32_t baseMipLevel)
	{
		m_BaseMipLevel = baseMipLevel;
		return *this;
	}

	FrameBuilder& FrameBuilder::SetLevelCount(uint32_t levelCount)
	{
		m_LevelCount = levelCount;
		return *this;
	}

	FrameBuilder& FrameBuilder::SetBaseArrayLayer(uint32_t baseArrayLayer)
	{
		m_BaseArrayLayer = baseArrayLayer;
		return *this;
	}

	FrameBuilder& FrameBuilder::SetLayerCount(uint32_t layerCount)
	{
		m_LayerCount = layerCount;
		return *this;
	}
}