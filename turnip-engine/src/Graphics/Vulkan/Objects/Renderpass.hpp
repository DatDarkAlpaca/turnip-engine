#pragma once
#include <vulkan/vulkan.hpp>

namespace tur::vulkan
{
	class RenderpassObject
	{
	public:
		RenderpassObject() = default;

	public:
		operator vk::RenderPass() { return renderpass; }

	public:
		vk::RenderPass renderpass;
		vk::Extent2D extent;
		vk::Offset2D offset;
	};
}