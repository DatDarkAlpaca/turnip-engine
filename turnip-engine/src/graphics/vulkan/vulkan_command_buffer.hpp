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
		CommandBufferVulkan(NON_OWNING GraphicsDeviceVulkan* device)
		{

		}

	protected:
		void initialize_impl()
		{

		}

	protected:
		void begin_impl()
		{

		}

		void end_impl()
		{

		}

	protected:
		void set_viewport_impl(const Viewport& viewport)
		{

		}
		void set_scissor_impl(const Rect2D& scissor)
		{

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

		}
		void draw_impl(u32 count, BufferIndexType type)
		{

		}
		void draw_instanced_impl(u32 first, u32 vertexCount, u32 instanceCount)
		{

		}

	private:
		NON_OWNING GraphicsDeviceVulkan* r_Device = nullptr;
	};
}