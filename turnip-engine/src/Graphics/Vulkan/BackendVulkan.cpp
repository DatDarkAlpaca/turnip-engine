#include "pch.h"
#include "BackendVulkan.h"
#include "Initialization/VulkanInitializer.h"

#include "ShaderVulkan.h"
#include "PipelineVulkan.h"

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

	tur_unique<Pipeline> BackendVulkan::CreatePipeline(const PipelineDescriptor& descriptor)
	{
		PipelineBuilder builder(descriptor);
		builder.SetDevice(m_Device)
			   .SetSwapchain(m_SwapchainData);

		PipelineVulkan pipeline = builder.Build();
		return tur::MakeUnique<PipelineVulkan>(pipeline);
	}
}
