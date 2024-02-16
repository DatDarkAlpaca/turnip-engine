#pragma once
#include <optional>
#include <vulkan/vulkan.hpp>

#include "Common.h"
#include "Rendering/Resource/Pipeline.h"
#include "Platform/Vulkan/Objects/Objects.h"

namespace tur::vulkan
{
	class PipelineBuilder
	{
	public:
		explicit PipelineBuilder(const PipelineStateDescriptor& descriptor)
			: descriptor(descriptor) { }

	public:
		PipelineVulkan Build();

	private:
		vk::PipelineLayout CreateLayout() const;

	public:
		PipelineBuilder& SetArguments(const vk::Device& device, const Swapchain& swapchain, const RenderpassVulkan& renderpass);

	private:
		RenderpassVulkan m_Renderpass;
		bool m_ArgumentsSet = false;

		// Device Input:
		vk::Device m_Device;

		// Swapchain Input:
		vk::Extent2D m_SwapchainExtent;
		vk::Format m_SwapchainFormat = vk::Format::eUndefined;

		PipelineStateDescriptor descriptor;
	};
}