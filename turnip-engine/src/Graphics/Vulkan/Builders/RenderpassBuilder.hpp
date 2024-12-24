#pragma once
#include <optional>
#include <vulkan/vulkan.hpp>

#include "Graphics/Base/Renderpass.hpp"
#include "Graphics/Vulkan/Objects/Swapchain.hpp"
#include "Graphics/Vulkan/Objects/Renderpass.hpp"

namespace tur::vulkan
{
	class RenderpassBuilder
	{
	public:
		RenderpassBuilder(const RenderpassDescriptor& descriptor);

	public:
		std::optional<RenderpassObject> Build();

	public:
		RenderpassBuilder& SetArguments(vk::Device device, const SwapchainObject& swapchain);

	private:
		void AddSwapchainColorAttachment();

		std::optional<RenderpassObject> ConfigureRenderpass();

	private:
		vk::Device m_Device;
		SwapchainObject m_Swapchain;
		bool m_ArgumentsSet = false;

	private:
		std::vector<vk::AttachmentDescription> m_Attachments;
		std::vector<vk::AttachmentReference> m_AttachmentReferences;
		std::vector<vk::SubpassDescription> m_Subpasses;
		RenderpassDescriptor m_Descriptor;
	};
}