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
		RenderpassBuilder(const RenderpassDescriptor& descriptor);

	public:
		std::optional<RenderpassVulkan> Build();

	public:
		RenderpassBuilder& SetArguments(vk::Device device, const vulkan::Swapchain& swapchain);

	private:
		void AddSwapchainColorAttachment();

		void PrepareDescriptor();

	private:
		vk::Device m_Device;
		vulkan::Swapchain m_Swapchain;

		bool m_ArgumentsSet = false;

	private:
		std::vector<vk::AttachmentDescription> m_Attachments;
		std::vector<vk::AttachmentReference> m_AttachmentReferences;
		std::vector<vk::SubpassDescription> m_Subpasses;
		RenderpassDescriptor m_Descriptor;
	};
}