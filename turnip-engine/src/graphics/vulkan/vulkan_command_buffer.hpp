#pragma once
#include <unordered_map>

#include "vulkan_device.hpp"
#include "graphics/command_buffer.hpp"

namespace tur::vulkan
{
	class CommandBufferVulkan : public BaseCommandBuffer<CommandBufferVulkan>
	{
		friend class BaseCommandBuffer<CommandBufferVulkan>;
		friend class VulkanGUI;

	public:
		CommandBufferVulkan(NON_OWNING GraphicsDeviceVulkan* device);

	protected:
		void initialize_impl();

	protected:
		void begin_render_impl(render_target_handle textureHandle);
		void end_render_impl();

	protected:
		void set_viewport_impl(const Viewport& viewport);
		void set_scissor_impl(const Rect2D& scissor);
		void clear_impl(ClearFlags flags, const ClearValue& clearValue);

	protected:
		void bind_pipeline_impl(pipeline_handle textureHandle);
		void bind_descriptor_set_impl(descriptor_set_handle textureHandle);
		void bind_vertex_buffer_impl(buffer_handle textureHandle, u32 binding, u32 stride);
		void bind_index_buffer_impl(buffer_handle textureHandle, BufferIndexType type);

	protected:
		void draw_impl(u32 vertexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance);
		void draw_indexed_impl(u32 indexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance);

	private:
		void transition_image(vk::Image targetImage, vk::ImageLayout currentLayout, vk::ImageLayout newLayout);
		void copy_image(vk::Image source, vk::Image target, vk::Extent2D sourceSize, vk::Extent2D targetSize);

	private:
		inline VulkanState& get_state()
		{
			return r_Device->get_state();
		}
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
		Pipeline m_BoundPipeline;
		ClearValue m_ClearValue;

		render_target_handle m_CurrentRenderTarget = invalid_handle;
	};
}