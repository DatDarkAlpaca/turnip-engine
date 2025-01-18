#pragma once
#include <glad/glad.h>
#include <unordered_map>

#include "opengl_device.hpp"
#include "graphics/command_buffer.hpp"

namespace tur::gl
{
	class CommandBufferGL : public BaseCommandBuffer<CommandBufferGL>
	{
		friend class BaseCommandBuffer<CommandBufferGL>;

	public:
		CommandBufferGL(NON_OWNING GraphicsDeviceGL* device);

	protected:
		void initialize_impl();

	protected:
		void begin_impl();

		void end_impl();

	protected:
		// virtual void begin_render_pass(renderpass_handle handle) { };
		// virtual void end_render_pass() { };

		// void set_viewport(const Rect2D& rect) { };
		void clear_impl(ClearFlags flags, const ClearValue& clearValue);

		void bind_pipeline_impl(pipeline_handle handle);
		void bind_vertex_buffer_impl(buffer_handle handle, uint32_t binding);
		void bind_index_buffer_impl(buffer_handle handle);
		void bind_texture_impl(texture_handle handle);

		void draw_impl(uint32_t first, uint32_t vertexCount);
		void draw_impl(uint32_t count, BufferIndexType type);
		
	private:
		void setup_pipeline_bindings(const PipelineDescriptor& descriptor);

	private:
		gl_handle m_VAO = invalid_handle;

		Buffer m_IndexBuffer;
		Pipeline m_ActivePipeline;
		std::unordered_map<int, int> m_BufferBindings;

	private:
		NON_OWNING GraphicsDeviceGL* r_Device = nullptr;
	};
}