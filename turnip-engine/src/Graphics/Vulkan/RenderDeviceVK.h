#pragma once
#include <glad/glad.h>
#include <vk_mem_alloc.h>

#include "Graphics/Vulkan/Objects/Objects.h"
#include "GraphicsCommandsVK.h"

#include "Rendering/RenderDevice.h"
#include "Platform/Platform.h"

namespace tur::vulkan
{
	class RenderDeviceVK : public RenderDevice
	{
	public:
		RenderDeviceVK(NON_OWNING Window* window, const GraphicsOptions& options);

		~RenderDeviceVK();

	public:
		inline GraphicsAPI API() override { return GraphicsAPI::VULKAN; }

	public:
		RenderpassHandle CreateRenderpass(const RenderpassDescriptor& renderpassDescription) override;

		BufferHandle CreateBuffer(const BufferDescriptor& bufferDescription) override;

		ShaderHandle CreateShader(const ShaderDescriptor& shaderDescriptor) override;

		PipelineStateHandle CreatePipeline(const PipelineStateDescriptor& pipelineDescriptor) override;

	public:
		tur_unique<GraphicsRenderCommands> CreateGraphicsCommands() override;

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

		inline vulkan::Buffer GetBuffer(BufferHandle handle)
		{
			return m_Buffers[static_cast<uint32_t>(handle)];
		}

		inline vulkan::RenderpassVulkan GetRenderpass(RenderpassHandle handle)
		{
			return m_Renderpasses[static_cast<uint32_t>(handle)];
		}

		inline void ClearShaders()
		{
			m_Shaders.clear();
		}

	public:
		inline Window* GetWindow() const { return r_Window; }

		inline GraphicsOptions GetOptions() const { return m_Options; }

	public:
		vk::Instance instance;
		vk::DebugUtilsMessengerEXT debugMessenger;
		vk::DispatchLoaderDynamic DLDI;
		vk::SurfaceKHR surface;
		vk::PhysicalDevice physicalDevice;
		vk::Device logicalDevice;
		VmaAllocator allocator = nullptr;
		vulkan::QueueCluster queues;
		vulkan::Swapchain swapchain;
		vk::CommandPool commandPool;

	private:
		std::vector<vulkan::RenderpassVulkan> m_Renderpasses;
		std::vector<vulkan::Shader> m_Shaders;
		std::vector<vulkan::Pipeline> m_Pipelines;
		std::vector<vulkan::Buffer> m_Buffers;

	private:
		NON_OWNING Window* r_Window = nullptr;
		GraphicsOptions m_Options;
	};
}
