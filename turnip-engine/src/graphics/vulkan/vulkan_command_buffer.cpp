#include "pch.hpp"
#include "vulkan_command_buffer.hpp"
#include "platform/vulkan_context.hpp"
#include "graphics/vulkan/objects/pipeline.hpp"

namespace tur::vulkan
{
	CommandBufferVulkan::CommandBufferVulkan(NON_OWNING GraphicsDeviceVulkan* device)
		: r_Device(device)
	{
	}

	void CommandBufferVulkan::initialize_impl()
	{
	}

	void CommandBufferVulkan::begin_render_impl(render_target_handle textureHandle)
	{
		m_CommandBuffer = get_command_buffer();
		m_CurrentRenderTarget = textureHandle;

		Texture* renderTargetTexture;
		if (textureHandle != invalid_handle)
		{
			auto renderTarget = r_Device->get_render_targets().get(textureHandle);
			renderTargetTexture = &r_Device->get_textures().get(renderTarget.descriptor.colorAttachments[0]);
		}
		else
			renderTargetTexture = &r_Device->get_state().drawTexture;

		transition_image(renderTargetTexture->image, renderTargetTexture->layout, vk::ImageLayout::eColorAttachmentOptimal);
		renderTargetTexture->layout = vk::ImageLayout::eColorAttachmentOptimal;

		vk::RenderingInfo renderInfo = {};
		{
			vk::RenderingAttachmentInfo colorAttachmentInfo = {};
			{
				colorAttachmentInfo.imageView = renderTargetTexture->imageView;
				colorAttachmentInfo.imageLayout = vk::ImageLayout::eAttachmentOptimal;
				colorAttachmentInfo.resolveMode = vk::ResolveModeFlagBits::eNone;
				colorAttachmentInfo.loadOp = vk::AttachmentLoadOp::eClear;
				colorAttachmentInfo.storeOp = vk::AttachmentStoreOp::eStore;
				colorAttachmentInfo.clearValue = vk::ClearValue({
					m_ClearValue.color.r,
					m_ClearValue.color.g,
					m_ClearValue.color.b,
					m_ClearValue.color.a
				});
			}

			/*vk::RenderingAttachmentInfo depthAttachmentInfo = {};
			{
				colorAttachmentInfo.imageView;
				colorAttachmentInfo.imageLayout;
				colorAttachmentInfo.resolveMode;
				colorAttachmentInfo.resolveImageView;
				colorAttachmentInfo.resolveImageLayout;
				colorAttachmentInfo.loadOp;
				colorAttachmentInfo.storeOp;
				colorAttachmentInfo.clearValue;
			}*/

			renderInfo.renderArea = vk::Rect2D({}, { renderTargetTexture->extent.width, renderTargetTexture->extent.height });
			renderInfo.colorAttachmentCount = 1;
			renderInfo.pColorAttachments = &colorAttachmentInfo;
			renderInfo.layerCount = 1;
			// render_info.pDepthAttachment = &depthAttachmentInfo;
			// render_info.pStencilAttachment = &depthAttachmentInfo;
		}

		m_CommandBuffer.beginRendering(renderInfo);
	}
	void CommandBufferVulkan::end_render_impl()
	{
		m_CommandBuffer.endRendering();
		
		auto& frameDataHolder = r_Device->get_state().frameDataHolder;
		auto& swapchainExtent = r_Device->get_state().swapchainExtent;
		auto& swapchainImages = r_Device->get_state().swapChainImages;

		const auto& currentImage = frameDataHolder.get_color_buffer();

		if (m_CurrentRenderTarget == invalid_handle)
		{
			auto& renderTarget = r_Device->get_state().drawTexture;

			transition_image(renderTarget.image, renderTarget.layout, vk::ImageLayout::eTransferSrcOptimal);
			renderTarget.layout = vk::ImageLayout::eTransferSrcOptimal;

			transition_image(swapchainImages.at(currentImage), vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);

			copy_image(renderTarget.image, swapchainImages.at(currentImage),
				{ renderTarget.extent.width, renderTarget.extent.height }, swapchainExtent);

			transition_image(swapchainImages.at(currentImage), vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::ePresentSrcKHR);
		}
	}

	void CommandBufferVulkan::set_viewport_impl(const Viewport& viewport)
	{
		m_CommandBuffer.setViewport(0, vk::Viewport(viewport.x, viewport.y, viewport.width, viewport.height, viewport.minDepth, viewport.maxDepth));
	}
	void CommandBufferVulkan::set_scissor_impl(const Rect2D& scissor)
	{
		m_CommandBuffer.setScissor(0,
			vk::Rect2D({ (i32)scissor.x, (i32)scissor.y }, { (u32)scissor.width, (u32)scissor.height }));
	}
	void CommandBufferVulkan::clear_impl(ClearFlags flags, const ClearValue& clearValue)
	{
		m_ClearValue = clearValue;
	}

	void CommandBufferVulkan::bind_pipeline_impl(pipeline_handle textureHandle)
	{
		auto& frameData = r_Device->get_state().frameDataHolder.get_frame_data();
		u32 frameNumber = r_Device->get_state().frameDataHolder.get_frame_number();

		Pipeline pipeline = r_Device->get_pipelines().get(textureHandle);
		m_CommandBuffer.bindPipeline(get_pipeline_type(pipeline.type), pipeline.pipeline);
		m_BoundPipeline = pipeline;
	}
	void CommandBufferVulkan::bind_descriptor_set_impl(descriptor_set_handle textureHandle)
	{
		m_CommandBuffer.bindDescriptorSets(
			get_pipeline_type(m_BoundPipeline.type),
			m_BoundPipeline.layout, 0,
			1, &r_Device->get_descriptor_sets().get(textureHandle).set,
			0, nullptr
		);
	}
	void CommandBufferVulkan::bind_vertex_buffer_impl(buffer_handle textureHandle, u32 binding, u32 stride)
	{
		const Buffer& buffer = r_Device->get_buffers().get(textureHandle);
		const vk::DeviceSize offset = 0;

		m_CommandBuffer.bindVertexBuffers(binding, buffer.buffer, offset);
	}
	void CommandBufferVulkan::bind_index_buffer_impl(buffer_handle textureHandle, BufferIndexType type = BufferIndexType::UNSIGNED_INT)
	{
		const Buffer& buffer = r_Device->get_buffers().get(textureHandle);
		const vk::DeviceSize offset = 0;

		m_CommandBuffer.bindIndexBuffer(buffer.buffer, offset, get_buffer_index_type(type));
	}
	
	void CommandBufferVulkan::draw_impl(u32 vertexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance)
	{
		m_CommandBuffer.draw(vertexCount, instanceCount, firstVertex, firstInstance);
	}
	void CommandBufferVulkan::draw_indexed_impl(u32 indexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance)
	{
		m_CommandBuffer.drawIndexed(indexCount, instanceCount, 0, firstVertex, firstInstance);
	}
}

namespace tur::vulkan
{
	void CommandBufferVulkan::transition_image(vk::Image targetImage, vk::ImageLayout currentLayout, vk::ImageLayout newLayout)
	{
		vk::ImageAspectFlags aspectMask;
		vk::ImageSubresourceRange subresourceRange;

		vk::ImageMemoryBarrier2 imageBarrier = {};
		{
			imageBarrier.srcStageMask = vk::PipelineStageFlagBits2::eAllCommands;
			imageBarrier.srcAccessMask = vk::AccessFlagBits2::eMemoryRead;

			imageBarrier.dstStageMask = vk::PipelineStageFlagBits2::eAllCommands;
			imageBarrier.dstAccessMask = vk::AccessFlagBits2::eMemoryWrite | vk::AccessFlagBits2::eMemoryRead;

			imageBarrier.oldLayout = currentLayout;
			imageBarrier.newLayout = newLayout;

			if (newLayout == vk::ImageLayout::eDepthAttachmentOptimal)
				aspectMask = vk::ImageAspectFlagBits::eDepth;
			else
				aspectMask = vk::ImageAspectFlagBits::eColor;

			{
				subresourceRange.aspectMask = aspectMask;
				subresourceRange.baseMipLevel = 0;
				subresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
				subresourceRange.baseArrayLayer = 0;
				subresourceRange.layerCount = VK_REMAINING_ARRAY_LAYERS;
			}

			imageBarrier.subresourceRange = subresourceRange;
			imageBarrier.image = targetImage;
		}

		vk::DependencyInfo dependencyInfo = vk::DependencyInfo().setImageMemoryBarriers(imageBarrier);

		try
		{
			m_CommandBuffer.pipelineBarrier2(dependencyInfo);
		}
		catch (vk::SystemError& err)
		{
			TUR_LOG_CRITICAL("Failed to issue transition image layout command. {}", err.what());
		}
	}
	void CommandBufferVulkan::copy_image(vk::Image source, vk::Image target, vk::Extent2D sourceSize, vk::Extent2D targetSize)
	{
		vk::ImageBlit2 blitRegion = {};
		{
			blitRegion.srcOffsets[1].x = sourceSize.width;
			blitRegion.srcOffsets[1].y = sourceSize.height;
			blitRegion.srcOffsets[1].z = 1;

			blitRegion.dstOffsets[1].x = targetSize.width;
			blitRegion.dstOffsets[1].y = targetSize.height;
			blitRegion.dstOffsets[1].z = 1;

			blitRegion.srcSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
			blitRegion.srcSubresource.baseArrayLayer = 0;
			blitRegion.srcSubresource.layerCount = 1;
			blitRegion.srcSubresource.mipLevel = 0;

			blitRegion.dstSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
			blitRegion.dstSubresource.baseArrayLayer = 0;
			blitRegion.dstSubresource.layerCount = 1;
			blitRegion.dstSubresource.mipLevel = 0;
		}

		vk::BlitImageInfo2 blitInfo = {};
		{
			blitInfo.srcImage = source;
			blitInfo.dstImage = target;
			blitInfo.srcImageLayout = vk::ImageLayout::eTransferSrcOptimal;
			blitInfo.dstImageLayout = vk::ImageLayout::eTransferDstOptimal;
			
			blitInfo.filter = vk::Filter::eLinear;
			blitInfo.regionCount = 1;
			blitInfo.pRegions = &blitRegion;
		}

		m_CommandBuffer.blitImage2(blitInfo);
	}
}