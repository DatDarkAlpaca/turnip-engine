#pragma once
#include <unordered_map>

#include "vulkan_device.hpp"
#include "graphics/command_buffer.hpp"

namespace tur::vulkan
{
	class CommandBufferVulkan : public BaseCommandBuffer<CommandBufferVulkan>
	{
		friend class BaseCommandBuffer<CommandBufferVulkan>;

	public:
		CommandBufferVulkan(NON_OWNING GraphicsDeviceVulkan* device);

	protected:
		void initialize_impl();

	protected:
		void begin_impl();
		void begin_render_impl();
		void end_render_impl();
		void end_impl();

	protected:
		void set_viewport_impl(const Viewport& viewport)
		{
			m_CommandBuffer.setViewport(0, vk::Viewport(viewport.x, viewport.y, viewport.width, viewport.height));
		}
		void set_scissor_impl(const Rect2D& scissor)
		{
			m_CommandBuffer.setScissor(0, 
				vk::Rect2D({ (i32)scissor.x, (i32)scissor.y }, { (u32)scissor.width, (u32)scissor.height }));
		}
		void clear_impl(ClearFlags flags, const ClearValue& clearValue)
		{

		}

		void update_buffer_impl(buffer_handle handle, u32 offset, const DataBuffer& data)
		{

		}

		void bind_pipeline_impl(pipeline_handle handle)
		{

		}
		void bind_vertex_buffer_impl(buffer_handle handle, u32 binding)
		{

		}
		void bind_index_buffer_impl(buffer_handle handle)
		{

		}
		void bind_uniform_buffer_impl(buffer_handle handle = invalid_handle)
		{

		}
		void bind_texture_impl(texture_handle handle, u32 textureUnit)
		{

		}
		void bind_descriptors_impl(buffer_handle handle, uint32_t binding)
		{

		}

		void push_constants_impl(u32 offset, PipelineStage stages, const DataBuffer& data)
		{

		}

		void draw_impl(u32 first, u32 vertexCount)
		{
			m_CommandBuffer.draw(vertexCount, 1, first, 0);
		}
		void draw_impl(u32 count, BufferIndexType type)
		{
			m_CommandBuffer.drawIndexed(count, 1, 0, 0, 0);
		}
		void draw_instanced_impl(u32 first, u32 vertexCount, u32 instanceCount)
		{
			// TODO: add instanceFirst to function parameters
			m_CommandBuffer.draw(vertexCount, instanceCount, first, 0);
		}

	private:
		void transition_image(vk::Image targetImage, vk::ImageLayout currentLayout, vk::ImageLayout newLayout)
		{
			vk::ImageAspectFlags aspectMask;
			vk::ImageSubresourceRange subresourceRange;

			vk::ImageMemoryBarrier2 imageBarrier = {};
			{
				imageBarrier.srcStageMask = vk::PipelineStageFlagBits2::eAllCommands;
				imageBarrier.srcAccessMask = vk::AccessFlagBits2::eMemoryWrite;

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

	private:
		inline FrameData& get_frame_data()
		{
			return r_Device->get_state().frameDataHolder.get_frame_data();
		}
		
		inline vk::Device& get_device() 
		{
			return r_Device->get_state().logicalDevice;
		}
		
		inline vk::CommandBuffer get_command_buffer()
		{
			return get_frame_data().commandBuffer;
		}	
		
	private:
		NON_OWNING GraphicsDeviceVulkan* r_Device = nullptr;
		vk::CommandBuffer m_CommandBuffer;
	};
}