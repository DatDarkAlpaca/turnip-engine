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
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);
	}

	void CommandBufferGL::begin_impl()
	{
		// * Blank
	}

	void CommandBufferGL::end_impl()
	{
		// * Blank
	}


	void CommandBufferGL::clear_impl(ClearFlags flags, const ClearValue& clearValue)
	{
		gl_handle setBits = 0;

		if (static_cast<uint32_t>(flags) & static_cast<uint32_t>(ClearFlags::COLOR))
		{
			glClearColor(clearValue.color.r, clearValue.color.g, clearValue.color.b, clearValue.color.a);
			setBits |= GL_COLOR_BUFFER_BIT;
		}

		if (static_cast<uint32_t>(flags) & static_cast<uint32_t>(ClearFlags::DEPTH))
		{
			glClearDepth(clearValue.depth);
			setBits |= GL_DEPTH_BUFFER_BIT;
		}

		if (static_cast<uint32_t>(flags) & static_cast<uint32_t>(ClearFlags::STENCIL))
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

	void CommandBufferGL::bind_vertex_buffer_impl(buffer_handle handle, uint32_t binding)
	{
		gl_handle bufferHandle = r_Device->get_buffers().get(handle).handle;
		m_BufferBindings[binding] = bufferHandle;
	}

	void CommandBufferGL::bind_index_buffer_impl(buffer_handle handle)
	{
		Buffer buffer = r_Device->get_buffers().get(handle);
		m_IndexBuffer = buffer;
	}

	void CommandBufferGL::bind_texture_impl(texture_handle handle)
	{
	}


	void CommandBufferGL::draw_impl(uint32_t first, uint32_t vertexCount)
	{
		gl_handle topology = get_primitive_topology(m_ActivePipeline.descriptor.inputAssemblyStage.topology);
		glDrawArrays(topology, first, vertexCount);
	}

	void CommandBufferGL::draw_impl(uint32_t count, BufferIndexType type)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer.handle);

		gl_handle topology = get_primitive_topology(m_ActivePipeline.descriptor.inputAssemblyStage.topology);
		glDrawElements(topology, count, get_buffer_index_type(type), nullptr);
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

				glEnableVertexAttribArray(attribute.location);

				gl_handle bufferHandle = m_BufferBindings[binding.binding];
				glBindBuffer(GL_ARRAY_BUFFER, bufferHandle);

				glVertexAttribPointer(
					attribute.location,
					get_format_size(attribute.format),
					get_format(attribute.format),
					attribute.normalized,
					binding.stride,
					(void*)attribute.offset
				);

				int divisor = binding.inputRate == InputRate::VERTEX ? 0 : 1;
				glVertexAttribDivisor(attribute.location, divisor);
			}
		}
	}
}