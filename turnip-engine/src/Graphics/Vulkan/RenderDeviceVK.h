#pragma once
#include <glad/glad.h>

#include "Graphics/Vulkan/Objects/Objects.h"
#include "GraphicsCommandsVK.h"

#include "Rendering/RenderDevice.h"
#include "Platform/Platform.h"

namespace tur::vulkan
{
	class RenderDeviceVK : public RenderDevice
	{
	public:
		RenderDeviceVK(NON_OWNING Window* window);

	public:
		RenderpassHandle CreateRenderpass(const RenderpassDescriptor& renderpassDescription) override;

		BufferHandle CreateBuffer(const BufferDescriptor& bufferDescription) override;

		ShaderHandle CreateShader(const ShaderDescriptor& shaderDescriptor) override;

		PipelineStateHandle CreatePipeline(const PipelineStateDescriptor& pipelineDescriptor) override;

	public:
		tur_unique<GraphicsRenderCommands> CreateGraphicsCommands() override
		{
			return tur::MakeUnique<vulkan::GraphicsRenderCommandsVK>(this);
		}

	public:
		Barrier Submit(RenderCommands* context) override;

		void WaitBarrier(const Barrier& barrier);

		void Present() override;

	public:
		void FinishSetup();

	public:
		inline vulkan::Shader GetShader(ShaderHandle handle)
		{
			return m_Shaders[static_cast<uint32_t>(handle)];
		}

		inline vulkan::Pipeline GetPipeline(PipelineStateHandle handle)
		{
			return m_Pipelines[static_cast<uint32_t>(handle)];
		}

		inline vulkan::RenderpassVulkan GetRenderpass(RenderpassHandle handle)
		{
			return m_Renderpasses[static_cast<uint32_t>(handle)];
		}

	public:
		inline Window* GetWindow() const { return r_Window; }

	public:
		vk::Instance instance;
		vk::DebugUtilsMessengerEXT debugMessenger;
		vk::SurfaceKHR surface;
		vk::PhysicalDevice physicalDevice;
		vk::Device logicalDevice;
		vulkan::QueueCluster queues;
		vulkan::Swapchain swapchain;
		vk::CommandPool commandPool;

	private:
		std::vector<vulkan::RenderpassVulkan> m_Renderpasses;
		std::vector<vulkan::Shader> m_Shaders;
		std::vector<vulkan::Pipeline> m_Pipelines;

	private:
		NON_OWNING Window* r_Window = nullptr;
	};
}
