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

		void set_viewport_impl(const Viewport& viewport);
		void set_scissor_impl(const Rect2D& scissor);
		void clear_impl(ClearFlags flags, const ClearValue& clearValue);

		void bind_pipeline_impl(pipeline_handle handle);
		void bind_vertex_buffer_impl(buffer_handle handle, u32 binding);
		void bind_index_buffer_impl(buffer_handle handle);
		void bind_uniform_buffer_impl(buffer_handle handle = invalid_handle);
		void bind_texture_impl(texture_handle handle);

		void push_constants_impl(u32 offset, PipelineStage stages, const DataBuffer& data);

		void draw_impl(u32 first, u32 vertexCount);
		void draw_impl(u32 count, BufferIndexType type);
		
	private:
		void setup_pipeline_bindings(const PipelineDescriptor& descriptor);

	private:
		NON_OWNING GraphicsDeviceGL* r_Device = nullptr;

	private:
		std::unordered_map<int, int> m_BufferBindings;
		Pipeline m_ActivePipeline;
		Buffer m_IndexBuffer;

		std::unordered_map<pipeline_handle, buffer_handle> m_PushConstantsBuffers;
		buffer_handle m_ActivePushConstantBuffer;

		gl_handle m_VAO = invalid_handle;
	};
}