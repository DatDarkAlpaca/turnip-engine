#include "pch.hpp"
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

	void CommandBufferGL::begin_impl()
	{
		glBindVertexArray(m_VAO);
	}
	void CommandBufferGL::begin_render_impl()
	{
		// * Blank
	}
	void CommandBufferGL::end_render_impl()
	{
		// * Blank
	}
	void CommandBufferGL::end_impl()
	{
		// * Blank
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
	
	void CommandBufferGL::bind_pipeline_impl(pipeline_handle handle)
	{
		m_ActivePipeline = r_Device->get_pipelines().get(handle);
		const auto& descriptor = m_ActivePipeline.descriptor;

		glUseProgram(m_ActivePipeline.handle);

		glFrontFace(get_front_face(descriptor.rasterizerStage.frontFace));
		glPolygonMode(GL_FRONT_AND_BACK, get_polygon_mode(descriptor.rasterizerStage.polygonMode));
		glCullFace(get_cull_mode(descriptor.rasterizerStage.cullMode));

		setup_pipeline_bindings(descriptor);
	}
	void CommandBufferGL::bind_vertex_buffer_impl(buffer_handle handle, u32 binding, u32 stride)
	{
		gl_handle bufferHandle = r_Device->get_buffers().get(handle).handle;
		glVertexArrayVertexBuffer(m_VAO, binding, bufferHandle, 0, stride);
	}
	void CommandBufferGL::bind_index_buffer_impl(buffer_handle handle, BufferIndexType indexType)
	{
		Buffer buffer = r_Device->get_buffers().get(handle);
		m_IndexType = indexType;
		glVertexArrayElementBuffer(m_VAO, buffer.handle);
	}
	void CommandBufferGL::bind_texture_impl(texture_handle handle, u32 textureUnit)
	{
		Texture texture = r_Device->get_textures().get(handle);
		glBindTextureUnit(textureUnit, texture.handle);
	}

	void CommandBufferGL::set_descriptor_resource_impl(handle_type handle, DescriptorType type, u32 binding)
	{
		gl_handle glType = get_descriptor_set_type(type);

		switch (type)
		{
			case DescriptorType::STORAGE_BUFFER:
			case DescriptorType::UNIFORM_BUFFER:
				glBindBufferBase(glType, binding, r_Device->get_buffers().get(handle).handle);
				break;

			case DescriptorType::COMBINED_IMAGE_SAMPLER:
				glBindTextureUnit(binding, r_Device->get_textures().get(handle).handle);
				break;
		}
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
	
	void CommandBufferGL::submit_impl()
	{
		/* Blank */
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