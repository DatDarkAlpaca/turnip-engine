#pragma once
#include <glad/glad.h>
#include <unordered_map>
#include "graphics/command_buffer.hpp"

namespace tur::gl
{
	class GraphicsDeviceGL;

	class CommandBufferGL : public BaseCommandBuffer<CommandBufferGL>
	{
		friend class BaseCommandBuffer<CommandBufferGL>;

	public:
		CommandBufferGL(NON_OWNING GraphicsDeviceGL* device);

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
		void bind_index_buffer_impl(buffer_handle textureHandle, BufferIndexType indexType);
		
	protected:
		void draw_impl(u32 vertexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance);
		void draw_indexed_impl(u32 indexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance);

	private:
		void setup_pipeline_bindings(const PipelineDescriptor& descriptor);

	private:
		NON_OWNING GraphicsDeviceGL* r_Device = nullptr;

	private:
		std::unordered_map<pipeline_handle, buffer_handle> m_PushConstantsBuffers;

		Pipeline m_ActivePipeline;
		buffer_handle m_ActivePushConstantBuffer = invalid_handle;
		gl_handle m_VAO = invalid_handle;

		BufferIndexType m_IndexType;
	};
}