#include "pch.h"
#include "BackendVulkan.h"
#include "VulkanInitializer.h"

#include "ShaderVulkan.h"

#include "Objects/Pipeline.h"
#include "Builders/PipelineBuilder.h"
#include "Builders/RenderpassBuilder.h"

namespace tur
{
	BackendVulkan::BackendVulkan(const BackendProperties& properties)
		: m_Properties(properties)
	{
		platform::ConfigureVulkan();
	}

	void BackendVulkan::FinishSetup(tur_unique<Window>& window)
	{
		m_Window = window.get();
	}

	tur_unique<Shader> BackendVulkan::CreateShader(const ShaderDescriptor& descriptor)
	{
		return tur::MakeUnique<ShaderVulkan>(m_Device, descriptor);
	}

	tur_unique<Renderpass> BackendVulkan::CreateRenderpass(const RenderpassDescriptor& descriptor)
	{
		vulkan::RenderpassBuilder builder(descriptor);
		builder.SetDevice(m_Device);
		builder.AddSwapchainColorAttachment(m_Swapchain); // Remove.

		auto renderpassResult = builder.Build();
		if (!renderpassResult.has_value())
			TUR_LOG_CRITICAL("Failed to create rendepass.");

		vulkan::RenderpassVulkan renderpass = renderpassResult.value();

		return tur::MakeUnique<vulkan::RenderpassVulkan>(renderpass);
	}

	tur_unique<Pipeline> BackendVulkan::CreatePipeline(const PipelineDescriptor& descriptor)
	{
		vulkan::PipelineBuilder builder(descriptor);

		vulkan::RenderpassVulkan renderpassFinal;

		if (!descriptor.renderpass)
		{
			// TODO: Create default graphics pipeline renderpass using the swapchain
			vulkan::RenderpassBuilder builder;
			builder.SetDevice(m_Device);
			builder.AddSwapchainColorAttachment(m_Swapchain);

			renderpassFinal = builder.Build().value();
		}
		else
		{
			renderpassFinal = *(vulkan::RenderpassVulkan*)descriptor.renderpass;
		}

		builder.SetArguments(m_Device, m_Swapchain, renderpassFinal);

		vulkan::PipelineVulkan pipeline = builder.Build();

		// Finish setup:
		vulkan::FramebufferBuilder frameBuilder;
		frameBuilder.SetArguments(m_Device, renderpassFinal.renderpass);

		for (auto& frame : m_Swapchain.frames)
			frame.framebuffer = frameBuilder.Create(frame.view, m_Swapchain);

		return tur::MakeUnique<vulkan::PipelineVulkan>(pipeline);
	}
}
