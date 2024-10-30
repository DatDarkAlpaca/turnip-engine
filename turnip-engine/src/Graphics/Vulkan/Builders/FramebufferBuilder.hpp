#pragma once
#include <vulkan/vulkan.hpp>
#include "Graphics/Vulkan/Objects/Swapchain.h"

namespace tur::vulkan
{
	class FramebufferBuilder
	{
	public:
		FramebufferBuilder() = default;

	public:
		FramebufferBuilder& SetArguments(vk::Device device, vk::RenderPass renderpass);

	public:
		vk::Framebuffer Create(const vk::ImageView& view, const Swapchain& swapchain);

	private:
		vk::Device m_Device;
		vk::RenderPass m_Renderpass;
		bool m_ArgumentsSet = false;
	};
}