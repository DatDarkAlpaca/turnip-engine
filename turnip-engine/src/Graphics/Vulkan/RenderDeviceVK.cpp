#include "pch.h"
#include "Util/File.h"
#include "RenderDeviceVK.h"

#include "Builders/FramebufferBuilder.h"
#include "Builders/RenderpassBuilder.h"
#include "Builders/PipelineBuilder.h"
#include "Builders/SyncBuilder.h"
#include "Builders/CommandBuilder.h"
#include "Builders/BufferBuilder.h"

namespace tur::vulkan
{
	RenderDeviceVK::RenderDeviceVK(NON_OWNING Window* window)
		: r_Window(window)
	{

	}

	RenderDeviceVK::~RenderDeviceVK()
	{
		logicalDevice.waitIdle();

		// Main Command Pool:
		logicalDevice.destroyCommandPool(commandPool);

		// Graphics Pipelines:
		{
			for (const auto& pipeline : m_Pipelines)
			{
				logicalDevice.destroyPipeline(pipeline.pipeline);
				logicalDevice.destroyPipelineLayout(pipeline.layout);
			}

			for (const auto& renderpass : m_Renderpasses)
				logicalDevice.destroyRenderPass(renderpass.renderpass);
		}
	
		// Frames:
		{
			for (const auto& frame : swapchain.frames) 
			{
				logicalDevice.destroyImageView(frame.view);
				logicalDevice.destroyFramebuffer(frame.framebuffer);
				logicalDevice.destroyFence(frame.inFlightFence);
				logicalDevice.destroySemaphore(frame.imageAvailableSemaphore);
				logicalDevice.destroySemaphore(frame.renderFinishedSemaphore);
			}

			logicalDevice.destroySwapchainKHR(swapchain.swapchain);
		}
		
		// Buffers.
		{
			for (const auto& buffer : m_Buffers)
				vmaDestroyBuffer(allocator, buffer.buffer, buffer.allocation);
		}

		logicalDevice.destroy();

		instance.destroySurfaceKHR(surface);
		instance.destroy();
	}

	RenderpassHandle RenderDeviceVK::CreateRenderpass(const RenderpassDescriptor& renderpassDescription)
	{
		RenderpassBuilder builder(renderpassDescription);
		auto renderpass = builder.SetArguments(logicalDevice, swapchain)
						         .Build();

		if (!renderpass.has_value())
		{
			TUR_LOG_WARN("Attempted to create renderpass with invalid arguments.");
			return RenderpassHandle::INVALID;
		}

		auto& renderpassData = renderpass.value();

		m_Renderpasses.push_back(renderpassData);		
		return RenderpassHandle(m_Renderpasses.size() - 1);
	}

	BufferHandle RenderDeviceVK::CreateBuffer(const BufferDescriptor& bufferDescription)
	{
		vulkan::BufferBuilder builder;
		vulkan::Buffer buffer = builder.Build(allocator, bufferDescription);

		m_Buffers.push_back(buffer);
		return BufferHandle(m_Buffers.size() - 1);
	}

	ShaderHandle RenderDeviceVK::CreateShader(const ShaderDescriptor& shaderDescriptor)
	{
		auto sourceCode = ReadBinaryFile(shaderDescriptor.filepath);

		vk::ShaderModuleCreateInfo moduleInfo = {};
		moduleInfo.flags = vk::ShaderModuleCreateFlags();
		moduleInfo.codeSize = sourceCode.size();
		moduleInfo.pCode = reinterpret_cast<const uint32_t*>(sourceCode.data());

		vulkan::Shader shader;

		try
		{
			shader.shaderModule = logicalDevice.createShaderModule(moduleInfo);
		}
		catch (const vk::SystemError& err)
		{
			TUR_LOG_ERROR("Failed to create shader module from filepath: {}. {}", shaderDescriptor.filepath, err.what());
		}

		m_Shaders.push_back(shader);
		return ShaderHandle(m_Shaders.size() - 1);
	}

	PipelineStateHandle RenderDeviceVK::CreatePipeline(const PipelineStateDescriptor& pipelineDescriptor)
	{
		// Default Renderpass:
		vulkan::RenderpassVulkan renderpass;
		if (pipelineDescriptor.renderpass == RenderpassHandle::INVALID)
			renderpass = m_Renderpasses[0];
		else
			renderpass = m_Renderpasses[(uint32_t)pipelineDescriptor.renderpass];

		vulkan::PipelineBuilder builder(this, pipelineDescriptor);
		builder.SetArguments(logicalDevice, swapchain, renderpass);

		vulkan::Pipeline pipeline = builder.Build();
		m_Pipelines.push_back(pipeline);

		return PipelineStateHandle(m_Pipelines.size() - 1);
	}

	tur_unique<GraphicsRenderCommands> RenderDeviceVK::CreateGraphicsCommands()
	{
		return tur::MakeUnique<vulkan::GraphicsRenderCommandsVK>(this);
	}

	void RenderDeviceVK::FinishSetup()
	{
		// Clear renderpasses for safety:
		m_Renderpasses.clear();
		
		vulkan::RenderpassVulkan vulkanRenderpass;

		// Swapchain renderpass:
		{
			RenderpassDescriptor desc = RenderpassDescriptor::CreateDefaultRenderpass();
			desc.extent = Extent{ swapchain.extent.width, swapchain.extent.height };

			vulkan::RenderpassBuilder renderpassBuilder(desc);
			renderpassBuilder.SetArguments(logicalDevice, swapchain);
			vulkanRenderpass = renderpassBuilder.Build().value();
			m_Renderpasses.push_back(vulkanRenderpass);
			swapchain.renderpass = vulkanRenderpass;
		}
		
		// Commands:
		CommandPoolBuilder poolBuilder;
		poolBuilder.SetArguments(logicalDevice, queues);
		commandPool = poolBuilder.Build().value(); // TODO: check value
		
		vulkan::CommandBufferBuilder commandBuilder;
		commandBuilder.SetArguments(logicalDevice, commandPool);

		// Frames:
		vulkan::FramebufferBuilder frameBuilder;
		frameBuilder.SetArguments(logicalDevice, swapchain.renderpass);

		for (auto& frame : swapchain.frames)
		{
			frame.framebuffer = frameBuilder.Create(frame.view, swapchain);
			frame.commandBuffer = commandBuilder.Build().value(); // TODO: check value

			SemaphoreBuilder semaphoreBuilder;
			FenceBuilder fenceBuilder;

			// Sync objects:
			frame.inFlightFence = fenceBuilder.Build(logicalDevice);
			frame.renderFinishedSemaphore = semaphoreBuilder.Build(logicalDevice);
			frame.imageAvailableSemaphore = semaphoreBuilder.Build(logicalDevice);
		}
	}

	Barrier RenderDeviceVK::Submit(RenderCommands* context)
	{
		// TODO: Accept other render command types.

		auto& currentFrame = swapchain.frames[swapchain.currentFrame];
		auto& graphicsQueue = queues.Get(vulkan::QueueOperation::GRAPHICS);

		vk::SubmitInfo submitInfo = { };

		vk::Semaphore waitSemaphores[] = { currentFrame.imageAvailableSemaphore };
		vk::PipelineStageFlags waitStages[] = { 
			vk::PipelineStageFlagBits::eColorAttachmentOutput 
		};

		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		// Command Buffers:
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &currentFrame.commandBuffer;

		// Signal Semaphores:
		vk::Semaphore signalSemaphores[] = { currentFrame.renderFinishedSemaphore };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		try
		{
			graphicsQueue.submit(submitInfo, currentFrame.inFlightFence);
		}
		catch (const vk::SystemError& err)
		{
			TUR_LOG_ERROR("Failed to submit draw command buffer: {}", err.what());
		}

		return Barrier();
	}

	void RenderDeviceVK::WaitBarrier(const Barrier& barrier)
	{
	}

	void RenderDeviceVK::Present()
	{
		auto& currentFrame = swapchain.frames[swapchain.currentFrame];

		vk::Semaphore signalSemaphores[] = { currentFrame.renderFinishedSemaphore };

		vk::PresentInfoKHR presentInfo = { };
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		vk::SwapchainKHR swapChains[] = { swapchain.swapchain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;

		presentInfo.pImageIndices = &swapchain.currentFrame;

		// TODO: error handling
		vk::Result _ = queues.Get(vulkan::QueueOperation::PRESENT).presentKHR(presentInfo);
		++swapchain.currentFrame;
		swapchain.currentFrame %= swapchain.frames.size();
	}
}