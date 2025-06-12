#include "pch.hpp"
#include "opengl_device.hpp"
#include "opengl_pipeline.hpp"
#include "opengl_command_buffer.hpp"

namespace tur::gl
{
	CommandBufferGL::CommandBufferGL(NON_OWNING GraphicsDeviceGL* device)
		: r_Device(device)
	{
	
	}

	void CommandBufferGL::initialize_impl()
	{
		glCreateVertexArrays(1, &m_VAO);
	}

	void CommandBufferGL::begin_render_impl(render_target_handle textureHandle)
	{
		glBindVertexArray(m_VAO);

		if(textureHandle == invalid_handle)
			return glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		glBindFramebuffer(GL_FRAMEBUFFER, r_Device->get_render_targets().get(textureHandle).handle);
	}
	void CommandBufferGL::end_render_impl()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void CommandBufferGL::set_viewport_impl(const Viewport& viewport)
	{
		glViewport(
			static_cast<i32>(viewport.x), 
			static_cast<i32>(viewport.y), 
			static_cast<i32>(viewport.width), 
			static_cast<i32>(viewport.height));
	}
	void CommandBufferGL::set_scissor_impl(const Rect2D& scissor)
	{
		glScissor(
			static_cast<u32>(scissor.x), 
			static_cast<u32>(scissor.y), 
			static_cast<u32>(scissor.width), 
			static_cast<u32>(scissor.height)
		);
	}
	void CommandBufferGL::clear_impl(ClearFlags flags, const ClearValue& clearValue)
	{
		gl_handle setBits = 0;

		if (static_cast<u32>(flags) & static_cast<u32>(ClearFlags::COLOR))
		{
			glClearColor(clearValue.color.r, clearValue.color.g, clearValue.color.b, clearValue.color.a);
			setBits |= GL_COLOR_BUFFER_BIT;
		}

		if (static_cast<u32>(flags) & static_cast<u32>(ClearFlags::DEPTH))
		{
			glClearDepth(clearValue.depth);
			setBits |= GL_DEPTH_BUFFER_BIT;
		}

		if (static_cast<u32>(flags) & static_cast<u32>(ClearFlags::STENCIL))
		{
			glClearStencil(clearValue.stencil);
			setBits |= GL_STENCIL_BUFFER_BIT;
		}

		glClear(setBits);
	}
	
	void CommandBufferGL::bind_pipeline_impl(pipeline_handle textureHandle)
	{
		m_ActivePipeline = r_Device->get_pipelines().get(textureHandle);
		const auto& descriptor = m_ActivePipeline.descriptor;

		glUseProgram(m_ActivePipeline.handle);

		glFrontFace(get_front_face(descriptor.rasterizerStage.frontFace));
		glPolygonMode(GL_FRONT_AND_BACK, get_polygon_mode(descriptor.rasterizerStage.polygonMode));
		glCullFace(get_cull_mode(descriptor.rasterizerStage.cullMode));

		setup_pipeline_bindings(descriptor);
	}
	void CommandBufferGL::bind_descriptor_set_impl(descriptor_set_handle handle)
	{
		const auto& descriptorset = r_Device->get_descriptor_sets().get(handle);

		for (const auto& buffer : descriptorset.boundBuffers)
		{
		}

		for (const auto& texture : descriptorset.boundTextures)
		{
			glBindTexture(GL_TEXTURE_2D, r_Device->get_textures().get(texture).handle);
		}
	}
	void CommandBufferGL::bind_vertex_buffer_impl(buffer_handle textureHandle, u32 binding, u32 stride)
	{
		gl_handle bufferHandle = r_Device->get_buffers().get(textureHandle).handle;
		glVertexArrayVertexBuffer(m_VAO, binding, bufferHandle, 0, stride);
	}
	void CommandBufferGL::bind_index_buffer_impl(buffer_handle textureHandle, BufferIndexType indexType)
	{
		Buffer buffer = r_Device->get_buffers().get(textureHandle);
		m_IndexType = indexType;
		glVertexArrayElementBuffer(m_VAO, buffer.handle);
	}

	void CommandBufferGL::draw_impl(u32 vertexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance)
	{
		gl_handle topology = get_primitive_topology(m_ActivePipeline.descriptor.inputAssemblyStage.topology);
		
		if (instanceCount == 1)
			glDrawArrays(topology, firstVertex, vertexCount);
		else
			glDrawArraysInstanced(topology, firstVertex, vertexCount, instanceCount);
	}
	void CommandBufferGL::draw_indexed_impl(u32 indexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance)
	{
		gl_handle topology = get_primitive_topology(m_ActivePipeline.descriptor.inputAssemblyStage.topology);

		if (instanceCount == 1)
			glDrawElements(topology, indexCount, get_buffer_index_type(m_IndexType), nullptr);
		else
			glDrawElementsInstanced(topology, indexCount, get_buffer_index_type(m_IndexType), nullptr, instanceCount);
	}
}

namespace tur::gl
{
	void CommandBufferGL::setup_pipeline_bindings(const PipelineDescriptor& descriptor)
	{
		for (const auto& binding : descriptor.vertexInputStage.bindings)
		{
			for (const auto& attribute : descriptor.vertexInputStage.attributes)
			{
				if (attribute.binding != binding.binding)
					continue;
				
				glEnableVertexArrayAttrib(m_VAO, attribute.location);
				glVertexArrayAttribFormat(
					m_VAO,
					attribute.location,
					static_cast<i32>(get_attribute_format_size(attribute.format)),
					get_attribute_format(attribute.format),
					attribute.normalized,
					attribute.offset
				);

				glVertexArrayAttribBinding(m_VAO, attribute.location, binding.binding);

				u32 divisor = binding.inputRate == InputRate::VERTEX ? 0 : 1;
				glVertexArrayBindingDivisor(m_VAO, binding.binding, divisor);
			}
		}		
	}
}