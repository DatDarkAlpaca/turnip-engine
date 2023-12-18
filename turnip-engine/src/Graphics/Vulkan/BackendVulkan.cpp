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
	}

	void BackendVulkan::InitializeWindow(tur_unique<Window>& window)
	{
		platform::ConfigureVulkan();

		WindowProperties windowProperties = window.get() ? window->GetProperties() : WindowProperties{};
		window = tur::MakeUnique<Window>();
		window->Initialize(windowProperties);

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
