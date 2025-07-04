#pragma once
#include "common.hpp"
#include "type/types.hpp"
#include "objects/buffer.hpp"
#include "objects/texture.hpp"
#include "objects/pipeline.hpp"

namespace tur
{
	template<typename CommandBuffer>
	class BaseCommandBuffer
	{
	public:
		void initialize()
		{
			static_cast<CommandBuffer*>(this)->initialize_impl();
		}

	public:
		void begin_render(render_target_handle textureHandle = invalid_handle)
		{
			static_cast<CommandBuffer*>(this)->begin_render_impl(textureHandle);
		}
		void end_render()
		{
			static_cast<CommandBuffer*>(this)->end_render_impl();
		}

	public:
		void set_viewport(const Viewport& viewport)
		{
			static_cast<CommandBuffer*>(this)->set_viewport_impl(viewport);
		};
		void set_scissor(const Rect2D& scissor)
		{
			static_cast<CommandBuffer*>(this)->set_scissor_impl(scissor);
		};
		void clear(ClearFlags flags, const ClearValue& clearValue)
		{
			static_cast<CommandBuffer*>(this)->clear_impl(flags, clearValue);
		}

	public:
		void bind_pipeline(pipeline_handle textureHandle)
		{
			static_cast<CommandBuffer*>(this)->bind_pipeline_impl(textureHandle);
		}
		void bind_descriptor_set(descriptor_set_handle textureHandle)
		{
			static_cast<CommandBuffer*>(this)->bind_descriptor_set_impl(textureHandle);
		}
		void bind_vertex_buffer(buffer_handle textureHandle, u32 binding, u32 stride)
		{
			static_cast<CommandBuffer*>(this)->bind_vertex_buffer_impl(textureHandle, binding, stride);
		}
		void bind_index_buffer(buffer_handle textureHandle, BufferIndexType type = BufferIndexType::UNSIGNED_INT)
		{
			static_cast<CommandBuffer*>(this)->bind_index_buffer_impl(textureHandle, type);
		}

	public:
		void draw(u32 vertexCount, u32 instanceCount = 1, u32 firstVertex = 0, u32 firstInstance = 0)
		{
			static_cast<CommandBuffer*>(this)->draw_impl(vertexCount, instanceCount, firstVertex, firstInstance);
		};
		void draw_indexed(u32 indexCount, u32 instanceCount = 1, u32 firstVertex = 0, u32 firstInstance = 0)
		{
			static_cast<CommandBuffer*>(this)->draw_indexed_impl(indexCount, instanceCount, firstVertex, firstInstance);
		};

	protected:
		BaseCommandBuffer() = default;
	};
}