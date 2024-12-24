#pragma once
#include <vulkan/vulkan.hpp>
#include "Graphics/Vulkan/Objects/Objects.hpp"

namespace tur::vulkan
{
	class FrameBuilder
	{
	public:
		FrameBuilder();

	public:
		Frames Build(vk::Device device, SwapchainObject swapchain);

	public:
		FrameBuilder& SetViewType(vk::ImageViewType viewType);
		FrameBuilder& SetComponentSwizzle(const std::array<vk::ComponentSwizzle, 4>& swizzleFlags);
		FrameBuilder& SetComponentSwizzle(vk::ComponentSwizzle r, vk::ComponentSwizzle g, vk::ComponentSwizzle b, vk::ComponentSwizzle a);
		FrameBuilder& SetAspectMask(vk::ImageAspectFlagBits imageAspect);

		FrameBuilder& SetBaseMipLevel(uint32_t baseMipLevel = 0);
		FrameBuilder& SetLevelCount(uint32_t levelCount = 1);
		FrameBuilder& SetBaseArrayLayer(uint32_t baseArrayLayer = 0);
		FrameBuilder& SetLayerCount(uint32_t layerCount = 1);

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