#include "pch.h"
#include "BackendVulkan.h"
#include "VulkanInitializer.h"

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
}
