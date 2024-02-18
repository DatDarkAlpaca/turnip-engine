#pragma once
#include <vulkan/vulkan.hpp>

namespace tur::vulkan
{
	class RenderpassVulkan
	{
	public:
		operator vk::RenderPass() { return renderpass; }

	public:
		vk::RenderPass renderpass;
	};
}