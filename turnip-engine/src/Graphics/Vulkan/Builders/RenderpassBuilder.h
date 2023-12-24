#pragma once
#include "Common.h"
#include <optional>
#include <vulkan/vulkan.hpp>

#include "Graphics/Vulkan/Converters.h"
#include "Graphics/Vulkan/Objects/Swapchain.h"
#include "Graphics/Vulkan/Objects/Renderpass.h"

namespace tur::vulkan
{
	class RenderpassBuilder
	{
	public:
		std::optional<RenderpassVulkan> Build();

	public:
		RenderpassBuilder& SetDevice(vk::Device device);

	public:
		// TODO: add DataFormat and substitute SwpachainFormat
		uint32_t AddAttachment(uint32_t sampleCount, const AttachmentOperations& operations, ImageLayout initialLayout, ImageLayout finalLayout);

		uint32_t AddAttachment(uint32_t sampleCount, const Swapchain& swapchain, const AttachmentOperations& operations, ImageLayout initialLayout, ImageLayout finalLayout);

		uint32_t AddDefaultColorAttachment(const Swapchain& swapchain);

		RenderpassBuilder& AddReference();

	private:
		vk::Device m_Device;
		bool m_DeviceSet = false;

	private:
		std::vector<vk::AttachmentDescription> m_Attachments;
		std::vector<vk::SubpassDescription> m_Subpasses;
	};
}