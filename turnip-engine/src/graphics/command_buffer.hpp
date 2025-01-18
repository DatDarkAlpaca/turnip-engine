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

		// void set_viewport(const Rect2D& rect) { };

		void clear(ClearFlags flags, const ClearValue& clearValue)
		{
			static_cast<CommandBuffer*>(this)->clear_impl(flags, clearValue);
		}

		void bind_pipeline(pipeline_handle handle)
		{
			static_cast<CommandBuffer*>(this)->bind_pipeline_impl(handle);
		}

		void bind_vertex_buffer(buffer_handle handle, uint32_t binding)
		{
			static_cast<CommandBuffer*>(this)->bind_vertex_buffer_impl(handle, binding);
		}

		void bind_index_buffer(buffer_handle handle)
		{
			static_cast<CommandBuffer*>(this)->bind_index_buffer_impl(handle);
		}

		void bind_texture(texture_handle handle)
		{
			static_cast<CommandBuffer*>(this)->bind_texture_impl(handle);
		}

		void draw(uint32_t first, uint32_t vertexCount) 
		{
			static_cast<CommandBuffer*>(this)->draw_impl(first, vertexCount);
		};
		void draw(uint32_t count, BufferIndexType type)
		{
			static_cast<CommandBuffer*>(this)->draw_impl(count, type);
		}
		// void draw_instanced(uint32_t first, uint32_t vertexCount, uint32_t instanceCount) { };
		// void draw_indexed(uint32_t indexCount, uint32_t instanceCount = 1) { };
		// void draw_indexed_instanced(uint32_t indexCount, uint32_t instanceCount) {} ;

		void submit(uint32_t first, uint32_t vertexCount)
		{
			static_cast<CommandBuffer*>(this)->submit_impl();
		};

	protected:
		BaseCommandBuffer() = default;
	};
}