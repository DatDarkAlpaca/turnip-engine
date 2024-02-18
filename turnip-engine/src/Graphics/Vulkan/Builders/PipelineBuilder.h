#pragma once
#include <optional>
#include <vulkan/vulkan.hpp>

#include "Common.h"
#include "Rendering/Resource/Pipeline.h"
#include "Graphics/Vulkan/RenderDeviceVK.h"
#include "Graphics/Vulkan/Objects/Objects.h"

namespace tur::vulkan
{
	class PipelineBuilder
	{
	public:
		explicit PipelineBuilder(NON_OWNING RenderDeviceVK* renderDevice, const PipelineStateDescriptor& descriptor)
			: r_RenderDevice(renderDevice)
			, descriptor(descriptor) { }

	public:
		Pipeline Build();

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

	private:
		NON_OWNING RenderDeviceVK* r_RenderDevice = nullptr;
	};
}