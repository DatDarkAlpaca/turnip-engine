#pragma once
#include <vulkan/vulkan.hpp>
#include "Graphics/Vulkan/Objects/Objects.h"

namespace tur::vulkan
{
	class SwapchainFrameBuilder
	{
	public:
		SwapchainFrameBuilder();

	public:
		void Build(vk::Device& device, Swapchain& swapchain);

	public:
		SwapchainFrameBuilder& SetViewType(vk::ImageViewType viewType);
		SwapchainFrameBuilder& SetComponentSwizzle(const std::array<vk::ComponentSwizzle, 4>& swizzleFlags);
		SwapchainFrameBuilder& SetComponentSwizzle(vk::ComponentSwizzle r, vk::ComponentSwizzle g, vk::ComponentSwizzle b, vk::ComponentSwizzle a);
		SwapchainFrameBuilder& SetAspectMask(vk::ImageAspectFlagBits imageAspect);

		SwapchainFrameBuilder& SetBaseMipLevel(uint32_t baseMipLevel = 0);
		SwapchainFrameBuilder& SetLevelCount(uint32_t levelCount = 1);
		SwapchainFrameBuilder& SetBaseArrayLayer(uint32_t baseArrayLayer = 0);
		SwapchainFrameBuilder& SetLayerCount(uint32_t layerCount = 1);

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