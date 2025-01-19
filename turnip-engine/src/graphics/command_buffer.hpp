#pragma once
#include "common.hpp"
#include "objects/buffer.hpp"
#include "objects/texture.hpp"
#include "objects/pipeline.hpp"
#include "type/types.hpp"

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

		void end()
		{
			static_cast<CommandBuffer*>(this)->end_impl();
		};

	public:
		// virtual void begin_render_pass(renderpass_handle handle) { };
		// virtual void end_render_pass() { };

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
		void bind_index_buffer(buffer_handle handle)
		{
			static_cast<CommandBuffer*>(this)->bind_index_buffer_impl(handle);
		}
		void bind_uniform_buffer(buffer_handle handle = invalid_handle)
		{
			static_cast<CommandBuffer*>(this)->bind_uniform_buffer_impl(handle);
		}
		void bind_texture(texture_handle handle)
		{
			static_cast<CommandBuffer*>(this)->bind_texture_impl(handle);
		}

		void push_constants(u32 offset, PipelineStage stages, const DataBuffer& data)
		{
			static_cast<CommandBuffer*>(this)->push_constants_impl(offset, stages, data);
		}

		void draw(u32 first, u32 vertexCount)
		{
			static_cast<CommandBuffer*>(this)->draw_impl(first, vertexCount);
		};
		void draw(u32 count, BufferIndexType type)
		{
			static_cast<CommandBuffer*>(this)->draw_impl(count, type);
		}
		// void draw_instanced(u32 first, u32 vertexCount, u32 instanceCount) { };
		// void draw_indexed(u32 indexCount, u32 instanceCount = 1) { };
		// void draw_indexed_instanced(u32 indexCount, u32 instanceCount) {} ;

		void submit(u32 first, u32 vertexCount)
		{
			static_cast<CommandBuffer*>(this)->submit_impl();
		};

	protected:
		BaseCommandBuffer() = default;
	};
}