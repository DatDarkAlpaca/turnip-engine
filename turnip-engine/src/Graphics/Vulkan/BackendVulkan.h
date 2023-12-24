#pragma once
#include "Core/Window/WindowProperties.h"
#include "Graphics/GraphicsBackend.h"
#include "Platform/Platform.h"

#include "Common.h"
#include "Objects/Queue.h"
#include "Objects/Swapchain.h"

#define VULKAN_BACKEND(IBackend) static_cast<BackendVulkan*>(IBackend)

namespace tur
{
	class BackendVulkan final : public IGraphicsBackend
	{
	public:
		BackendVulkan(const BackendProperties& properties);

		// TODO: Implement Destructor.

	public:
		void FinishSetup(tur_unique<Window>&) override;

		void Present() override { TUR_LOG_WARN("Present() not implemented on Vulkan"); }

	public:
		tur_unique<Shader> CreateShader(const ShaderDescriptor& descriptor) override;

		tur_unique<Renderpass> CreateRenderpass() override;

		tur_unique<Pipeline> CreatePipeline(const PipelineDescriptor& descriptor) override;

	public:
		EXPOSE_PROPERTY(BackendProperties, Properties, m_Properties);

		EXPOSE_PROPERTY(vk::Instance, Instance, m_Instance);
		EXPOSE_PROPERTY(vk::DebugUtilsMessengerEXT, DebugMessenger, m_DebugMessenger);
		EXPOSE_PROPERTY(vk::SurfaceKHR, SurfaceKHR, m_Surface);
		EXPOSE_PROPERTY(vk::PhysicalDevice, PhysicalDevice, m_PhysicalDevice);
		EXPOSE_PROPERTY(vk::Device, Device, m_Device);
		EXPOSE_PROPERTY(vulkan::QueueCluster, Queues, m_Queues);
		EXPOSE_PROPERTY(vulkan::Swapchain, Swapchain, m_Swapchain);

	public:
		Window* GetWindow() const { return m_Window; }

	private:
		vk::Instance m_Instance;
		vk::DebugUtilsMessengerEXT m_DebugMessenger;
		vk::SurfaceKHR m_Surface;
		vk::PhysicalDevice m_PhysicalDevice;
		vk::Device m_Device;
		vulkan::QueueCluster m_Queues;
		vulkan::Swapchain m_Swapchain;

	private:
		BackendProperties m_Properties;
		NON_OWNING Window* m_Window = nullptr;

	private:
		constexpr static inline uint32_t DefaultVersionMajor = 1;
		constexpr static inline uint32_t DefaultVersionMinor = 0;
	};
}