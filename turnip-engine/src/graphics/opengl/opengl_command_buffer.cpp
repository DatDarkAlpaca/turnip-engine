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
		glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
	}
	void CommandBufferGL::set_scissor_impl(const Rect2D& scissor)
	{
		glScissor(scissor.x, scissor.y, scissor.width, scissor.height);
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
	void CommandBufferGL::update_buffer_impl(buffer_handle handle, u32 offset, const DataBuffer& data)
	{
		Buffer buffer = r_Device->get_buffers().get(handle);
		const auto& type = get_buffer_type(buffer.descriptor.type);

		glBindBuffer(type, buffer.handle);
		glBufferSubData(type, offset, data.size, data.data);
		glBindBuffer(type, 0);
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

		// Descriptors:
		for (const auto& [binding, type, stages] : descriptor.pipelineLayout.bindingDescriptors)
		{
			if (type == BindingTypes::IMAGE_SAMPLER)
			{
				glActiveTexture(GL_TEXTURE0 + binding);
			}
		}

		// Push Constants:
#ifdef TUR_USE_PUSH_CONSTANTS
		const auto& pushConstants = m_ActivePipeline.descriptor.pipelineLayout.pushConstants;
		
		u32 totalSize = 0;
		for (const auto& constant : pushConstants)
			totalSize += constant.byteSize;

		// TODO: use cached push constants.
		BufferDescriptor pushDescriptor;
		{
			pushDescriptor.type = BufferType::UNIFORM_BUFFER;
			pushDescriptor.usage = BufferUsage::DYNAMIC;
		}

		if (m_PushConstantsBuffers.find(handle) == m_PushConstantsBuffers.end())
			m_PushConstantsBuffers[handle] = r_Device->create_buffer(pushDescriptor, totalSize);

		m_ActivePushConstantBuffer = m_PushConstantsBuffers[handle];

		glBindBufferRange(GL_UNIFORM_BUFFER, 0, r_Device->get_buffers().get(m_ActivePushConstantBuffer).handle, 0, totalSize);
#endif
	}
	void CommandBufferGL::bind_vertex_buffer_impl(buffer_handle handle, u32 binding)
	{
		gl_handle bufferHandle = r_Device->get_buffers().get(handle).handle;
		m_BufferBindings[binding] = bufferHandle;
	}
	void CommandBufferGL::bind_index_buffer_impl(buffer_handle handle)
	{
		Buffer buffer = r_Device->get_buffers().get(handle);
		m_IndexBuffer = buffer;
	}
	void CommandBufferGL::bind_uniform_buffer_impl(buffer_handle handle)
	{
		if (handle != invalid_handle)
			glBindBuffer(GL_UNIFORM_BUFFER, r_Device->get_buffers().get(handle).handle);
		else
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	void CommandBufferGL::bind_texture_impl(texture_handle handle, u32 textureUnit)
	{
		Texture texture = r_Device->get_textures().get(handle);

		if(textureUnit != invalid_handle)
			glActiveTexture(GL_TEXTURE0 + textureUnit);

		glBindTexture(get_texture_type(texture.descriptor.type), texture.handle);
	}
	void CommandBufferGL::bind_descriptors_impl(buffer_handle handle, uint32_t binding)
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, r_Device->get_buffers().get(handle).handle);
	}
	void CommandBufferGL::push_constants_impl(u32 offset, PipelineStage stages, const DataBuffer& data)
	{
		bind_uniform_buffer(m_ActivePushConstantBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, offset, data.size, data.data);
		bind_uniform_buffer();
	}

	void CommandBufferGL::draw_impl(u32 first, u32 vertexCount)
	{
		gl_handle topology = get_primitive_topology(m_ActivePipeline.descriptor.inputAssemblyStage.topology);
		glDrawArrays(topology, first, vertexCount);
	}
	void CommandBufferGL::draw_impl(u32 count, BufferIndexType type)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer.handle);

		gl_handle topology = get_primitive_topology(m_ActivePipeline.descriptor.inputAssemblyStage.topology);
		glDrawElements(topology, count, get_buffer_index_type(type), nullptr);
	}
	void CommandBufferGL::draw_instanced_impl(u32 first, u32 vertexCount, u32 instanceCount)
	{
		gl_handle topology = get_primitive_topology(m_ActivePipeline.descriptor.inputAssemblyStage.topology);
		glDrawArraysInstanced(topology, first, vertexCount, instanceCount);
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