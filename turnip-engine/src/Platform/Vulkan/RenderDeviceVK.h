#pragma once
#include <glad/glad.h>

#include "Platform/Vulkan/Objects/Objects.h"
#include "Rendering/RenderDevice.h"
#include "Platform/Platform.h"
#include "Util/File.h"

namespace tur
{
	class RenderDeviceVK : public RenderDevice
	{
	public:
		RenderDeviceVK(NON_OWNING Window* window)
			: r_Window(window)
		{

		}

	public:
		BufferHandle CreateBuffer(const BufferDescriptor& bufferDescription) override
		{
			// TODO: implement
			return BufferHandle::INVALID;
		}

		ShaderHandle CreateShader(const ShaderDescriptor& shaderDescriptor) override
		{
			// TODO: implement
			return ShaderHandle::INVALID;
		}

		PipelineStateHandle CreatePipeline(const PipelineStateDescriptor& pipelineDescriptor) override
		{
			// TODO: implement
			return PipelineStateHandle::INVALID;
		}

	public:
		void Present() override
		{
			// TODO: implement
		}

	public:
		Window* GetWindow() const { return r_Window; }

	private:
		NON_OWNING Window* r_Window = nullptr;

	public:
		vk::Instance instance;
		vk::DebugUtilsMessengerEXT debugMessenger;
		vk::SurfaceKHR surface;
		vk::PhysicalDevice physicalDevice;
		vk::Device logicalDevice;
		vulkan::QueueCluster queues;
		vulkan::Swapchain swapchain;
	};
}
