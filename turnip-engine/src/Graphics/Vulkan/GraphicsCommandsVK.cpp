#include "pch.h"
#include "RenderDeviceVK.h"
#include "GraphicsCommandsVK.h"

namespace tur::vulkan
{
	GraphicsRenderCommandsVK::GraphicsRenderCommandsVK(NON_OWNING RenderDeviceVK* renderDevice)
		: r_Device(renderDevice)
	{

	}

	void GraphicsRenderCommandsVK::Begin()
	{
		auto& device = r_Device->logicalDevice;
		auto& swapchain = r_Device->swapchain;
		auto& frames = r_Device->swapchain.frames;
		auto& currentFrame = swapchain.currentFrame;

		// Frame fence:
		auto _ = device.waitForFences(1, &frames[currentFrame].inFlightFence, VK_TRUE, UINT64_MAX);
		_ = device.resetFences(1, &frames[currentFrame].inFlightFence);

		// Fetch current frame information:
		uint32_t imageIndex = 0;
		imageIndex = device.acquireNextImageKHR(
			swapchain.swapchain,
			UINT64_MAX,
			frames[currentFrame].imageAvailableSemaphore, nullptr).value;

		currentFrame = imageIndex;
		m_CurrentBuffer = frames[currentFrame].commandBuffer;
		
		// Begin:
		vk::CommandBufferBeginInfo beginInfo = { };

		try
		{
			m_CurrentBuffer.begin(beginInfo);
		}
		catch (const vk::SystemError& err)
		{
			TUR_LOG_ERROR("Failed to begin command buffer recording state: {}", err.what());
		}
	}

	void GraphicsRenderCommandsVK::BeginRenderpass(RenderpassHandle handle)
	{
		vulkan::RenderpassVulkan renderpassVulkan = r_Device->GetRenderpass(handle);

		auto& swapchain = r_Device->swapchain;
		auto& currentFrame = swapchain.frames[swapchain.currentFrame];

		// TODO: make flexible render targets
		// Add extent in the render descriptor and vulkan renderpass
		vk::RenderPassBeginInfo renderPassInfo = { };
		renderPassInfo.renderPass = renderpassVulkan.renderpass;
		renderPassInfo.framebuffer = currentFrame.framebuffer;
		renderPassInfo.renderArea.offset.x = renderpassVulkan.offset.x;
		renderPassInfo.renderArea.offset.y = renderpassVulkan.offset.y;
		renderPassInfo.renderArea.extent = renderpassVulkan.extent;

		vk::ClearValue clearColor = { std::array<float, 4> {
			m_ClearColor.r,
			m_ClearColor.g,
			m_ClearColor.b,
			m_ClearColor.a
		} };

		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		m_CurrentBuffer.beginRenderPass(&renderPassInfo, vk::SubpassContents::eInline);
	}

	void GraphicsRenderCommandsVK::EndRenderpass()
	{
		m_CurrentBuffer.endRenderPass();
	}

	void GraphicsRenderCommandsVK::SetViewport()
	{
		vk::Viewport vulkanViewport { };

		auto dimensions = r_Device->GetWindow()->GetProperties().dimensions;

		vulkanViewport.width = dimensions.x;
		vulkanViewport.height = dimensions.y;
		vulkanViewport.x = 0;
		vulkanViewport.y = dimensions.y;

		if (r_Device->GetOptions().usingBottomLeftOrigin)
			vulkanViewport.height *= -1;

		m_CurrentBuffer.setViewport(0, { vulkanViewport });
	}

	void GraphicsRenderCommandsVK::SetViewport(const Viewport& viewport)
	{
		vk::Viewport vulkanViewport { };

		vulkanViewport.width = viewport.dimensions.x;
		vulkanViewport.height = viewport.dimensions.y;		
		vulkanViewport.x = viewport.origin.x;
		vulkanViewport.y = viewport.origin.y;

		if (r_Device->GetOptions().usingBottomLeftOrigin)
			vulkanViewport.height *= -1;

		m_CurrentBuffer.setViewport(0, { vulkanViewport });
	}

	void GraphicsRenderCommandsVK::SetClearColor(const glm::vec4& color)
	{
		m_ClearColor = color;
	}

	void GraphicsRenderCommandsVK::Clear(ClearFlags flags)
	{
		/* TODO: Implement explicly */
	}

	void GraphicsRenderCommandsVK::BindPipeline(PipelineStateHandle handle)
	{
		auto pipeline = r_Device->GetPipeline(handle);
		m_CurrentBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);
	}

	void GraphicsRenderCommandsVK::SetVertexBuffer(BufferHandle handle)
	{
		// TODO: make it flexible.
		auto buffer = r_Device->GetBuffer(handle);

		std::vector<vk::Buffer> buffers = { buffer };
		float offsets = { 0 };

		m_CurrentBuffer.bindVertexBuffers(0, buffers, offsets);
	}

	void GraphicsRenderCommandsVK::SetIndexBuffer(BufferHandle handle)
	{
		// TODO: make it flexible.
		auto buffer = r_Device->GetBuffer(handle);
		m_CurrentBuffer.bindIndexBuffer(buffer, 0, vk::IndexType::eUint32);
	}

	void GraphicsRenderCommandsVK::Draw(uint32_t first, uint32_t count)
	{
		m_CurrentBuffer.draw(count, 1, first, 0);
	}

	void GraphicsRenderCommandsVK::DrawIndexed(uint32_t count)
	{
		// TODO: make it flexible.
		m_CurrentBuffer.drawIndexed(count, 1, 0, 0, 0);
	}

	void GraphicsRenderCommandsVK::End()
	{
		auto& frames = r_Device->swapchain.frames;

		try
		{
			m_CurrentBuffer.end();
		}
		catch (const vk::SystemError& err)
		{
			TUR_LOG_ERROR("Failed to end buffer recordings: {}", err.what());
		}
	}
}

