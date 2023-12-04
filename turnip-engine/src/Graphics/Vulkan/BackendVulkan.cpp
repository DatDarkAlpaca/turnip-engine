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
		m_Window = window.get();

		platform::ConfigureVulkan();

		WindowProperties windowProperties = window.get() ? window->GetProperties() : WindowProperties{};
		window = tur::MakeUnique<Window>();
		window->Initialize(windowProperties);
	}
}
