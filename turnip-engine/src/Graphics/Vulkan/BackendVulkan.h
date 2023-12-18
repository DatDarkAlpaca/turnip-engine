#pragma once
#include "Core/Window/WindowProperties.h"
#include "Graphics/GraphicsBackend.h"
#include "Platform/Platform.h"

#include "Common.h"
#include "Initialization/Instance.h"
#include "Initialization/PhysicalDevice.h"
#include "Initialization/LogicalDevice.h"
#include "Initialization/Queues.h"
#include "Initialization/Swapchain.h"

#define VULKAN_BACKEND(VulkanBackendSmartPtr) static_cast<BackendVulkan*>(VulkanBackendSmartPtr.get())

namespace tur
{
	class BackendVulkan final : public IGraphicsBackend
	{
	public:
		BackendVulkan(const BackendProperties& properties);

		// TODO: Implement Destructor.

	public:
		void InitializeWindow(tur_unique<Window>& window) override;

		void Present() override { TUR_LOG_WARN("Not implemented"); }

	public:
		Shader* CreateShader(const ShaderDescriptor& descriptor) override;

	public:
		EXPOSE_PROPERTY(BackendProperties, Properties, m_Properties);

		EXPOSE_PROPERTY(vk::Instance, Instance, m_Instance);
		EXPOSE_PROPERTY(vk::DebugUtilsMessengerEXT, DebugMessenger, m_DebugMessenger);
		EXPOSE_PROPERTY(vk::SurfaceKHR, SurfaceKHR, m_Surface);
		EXPOSE_PROPERTY(vk::PhysicalDevice, PhysicalDevice, m_PhysicalDevice);
		EXPOSE_PROPERTY(vk::Device, Device, m_Device);
		EXPOSE_PROPERTY(vulkan::QueueInformation, Queues, m_Queues);
		EXPOSE_PROPERTY(vulkan::SwapchainData, SwapchainData, m_SwapchainData);

	public:
		Window* GetWindow() const { return m_Window; }

	private:
		vk::Instance m_Instance;
		vk::DebugUtilsMessengerEXT m_DebugMessenger;
		vk::SurfaceKHR m_Surface;
		vk::PhysicalDevice m_PhysicalDevice;
		vk::Device m_Device;
		vulkan::QueueInformation m_Queues;
		vulkan::SwapchainData m_SwapchainData;

	private:
		BackendProperties m_Properties;
		NON_OWNING Window* m_Window = nullptr;

	private:
		constexpr static inline uint32_t DefaultVersionMajor = 1;
		constexpr static inline uint32_t DefaultVersionMinor = 0;
	};
}