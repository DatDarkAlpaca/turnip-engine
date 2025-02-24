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
		void begin()
		{
			static_cast<CommandBuffer*>(this)->begin_impl();
		};
		void begin_render()
		{
			static_cast<CommandBuffer*>(this)->begin_render_impl();
		}
		void end_render()
		{
			static_cast<CommandBuffer*>(this)->end_render_impl();
		}
		void end()
		{
			static_cast<CommandBuffer*>(this)->end_impl();
		};

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

		void update_buffer(buffer_handle handle, u32 offset, const DataBuffer& data)
		{
			static_cast<CommandBuffer*>(this)->update_buffer_impl(handle, offset, data);
		}

		void bind_pipeline(pipeline_handle handle)
		{
			static_cast<CommandBuffer*>(this)->bind_pipeline_impl(handle);
		}
		void bind_vertex_buffer(buffer_handle handle, u32 binding)
		{
			static_cast<CommandBuffer*>(this)->bind_vertex_buffer_impl(handle, binding);
		}
		void bind_index_buffer(buffer_handle handle, BufferIndexType type = BufferIndexType::UNSIGNED_INT)
		{
			static_cast<CommandBuffer*>(this)->bind_index_buffer_impl(handle, type);
		}
		void bind_texture(texture_handle handle, u32 textureUnit = invalid_handle)
		{
			static_cast<CommandBuffer*>(this)->bind_texture_impl(handle, textureUnit);
		}
		void bind_descriptors(buffer_handle handle, uint32_t binding)
		{
			static_cast<CommandBuffer*>(this)->bind_descriptors_impl(handle, binding);
		}
			
		void draw(u32 vertexCount, u32 instanceCount = 1, u32 firstVertex = 0, u32 firstInstance = 0)
		{
			static_cast<CommandBuffer*>(this)->draw_impl(vertexCount, instanceCount, firstVertex, firstInstance);
		};
		void draw_indexed(u32 indexCount, u32 instanceCount = 1, u32 firstVertex = 0, u32 firstInstance = 0)
		{
			static_cast<CommandBuffer*>(this)->draw_indexed_impl(indexCount, instanceCount, firstVertex, firstInstance);
		};

		void submit()
		{
			static_cast<CommandBuffer*>(this)->submit_impl();
		}

	protected:
		BaseCommandBuffer() = default;
	};
}