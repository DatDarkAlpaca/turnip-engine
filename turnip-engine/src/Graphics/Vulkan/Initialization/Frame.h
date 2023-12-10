#pragma once
#include "Swapchain.h"

namespace tur::vulkan
{
	class SwapchainFrameBuilder
	{
	public:
		SwapchainFrameBuilder()
		{
			m_ComponentSwizzleFlags.fill(vk::ComponentSwizzle::eIdentity);
		}

	public:
		void Build(vk::Device& device, SwapchainData& swapchainResult)
		{
			auto swapchainImages = device.getSwapchainImagesKHR(swapchainResult.swapchain);

			for (size_t i = 0; i < swapchainImages.size(); ++i)
			{
				SwapchainFrame frameData;
				
				// Create ImageView:
				vk::ImageViewCreateInfo createInfo = {};
				createInfo.image = swapchainImages[i];
				createInfo.viewType = m_ViewType;
				createInfo.format = swapchainResult.surfaceFormat.format;
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

				swapchainResult.frames.push_back(frameData);
			}
		}

	public:
		SwapchainFrameBuilder& SetViewType(vk::ImageViewType viewType)
		{
			m_ViewType = viewType;
			return *this;
		}
		SwapchainFrameBuilder& SetComponentSwizzle(const std::array<vk::ComponentSwizzle, 4>& swizzleFlags)
		{
			m_ComponentSwizzleFlags = swizzleFlags;
			return *this;
		}
		SwapchainFrameBuilder& SetComponentSwizzle(vk::ComponentSwizzle r, vk::ComponentSwizzle g, vk::ComponentSwizzle b, vk::ComponentSwizzle a)
		{
			m_ComponentSwizzleFlags = { r,g,b,a };
			return *this;
		}
		SwapchainFrameBuilder& SetAspectMask(vk::ImageAspectFlagBits imageAspect)
		{
			m_ImageAspect = imageAspect;
			return *this;
		}

		SwapchainFrameBuilder& SetBaseMipLevel(uint32_t baseMipLevel = 0)
		{
			m_BaseMipLevel = baseMipLevel;
			return *this;
		}
		SwapchainFrameBuilder& SetLevelCount(uint32_t levelCount = 1)
		{
			m_LevelCount = levelCount;
			return *this;
		}
		SwapchainFrameBuilder& SetBaseArrayLayer(uint32_t baseArrayLayer = 0)
		{
			m_BaseArrayLayer = baseArrayLayer;
			return *this;
		}
		SwapchainFrameBuilder& SetLayerCount(uint32_t layerCount = 1)
		{
			m_LayerCount = layerCount;
			return *this;
		}

	private:
		vk::ImageViewType m_ViewType = vk::ImageViewType::e2D;
		std::array<vk::ComponentSwizzle, 4> m_ComponentSwizzleFlags;
		vk::ImageAspectFlagBits m_ImageAspect = vk::ImageAspectFlagBits::eColor;

		uint32_t m_BaseMipLevel = 0;
		uint32_t m_LevelCount = 1;
		uint32_t m_BaseArrayLayer = 0;
		uint32_t m_LayerCount = 1;
	};
}