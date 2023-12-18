#include "pch.h"
#include "BackendVulkan.h"
#include "Initialization/VulkanInitializer.h"

#include "ShaderVulkan.h"

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

	Shader* BackendVulkan::CreateShader(const ShaderDescriptor& descriptor)
	{
		return new ShaderVulkan(m_Device, descriptor);
	}
}
