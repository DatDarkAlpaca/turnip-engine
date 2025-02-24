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
		void begin_render_impl();
		void end_render_impl();
		void end_impl();

	protected:
		void set_viewport_impl(const Viewport& viewport);
		void set_scissor_impl(const Rect2D& scissor);
		void clear_impl(ClearFlags flags, const ClearValue& clearValue);

	protected:
		void bind_pipeline_impl(pipeline_handle handle);
		void bind_vertex_buffer_impl(buffer_handle handle, u32 binding);
		void bind_index_buffer_impl(buffer_handle handle, BufferIndexType type);
		void bind_texture_impl(texture_handle handle, u32 textureUnit);
		void bind_descriptors_impl(buffer_handle handle, uint32_t binding);

	protected:
		void draw_impl(u32 vertexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance);
		void draw_indexed_impl(u32 indexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance);

	protected:
		void update_buffer_impl(buffer_handle handle, u32 offset, const DataBuffer& data);

	protected:
		void submit_impl();

	private:
		void setup_pipeline_bindings(const PipelineDescriptor& descriptor);

	private:
		NON_OWNING GraphicsDeviceGL* r_Device = nullptr;

	private:
		std::unordered_map<int, int> m_BufferBindings;
		Pipeline m_ActivePipeline;
		Buffer m_IndexBuffer;

		std::unordered_map<pipeline_handle, buffer_handle> m_PushConstantsBuffers;
		buffer_handle m_ActivePushConstantBuffer = invalid_handle;

		BufferIndexType m_IndexType = BufferIndexType::UNSIGNED_INT;
		gl_handle m_VAO = invalid_handle;
	};
}