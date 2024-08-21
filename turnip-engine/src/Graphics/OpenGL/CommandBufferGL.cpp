#include "pch.hpp"
#include "TypeConverters.hpp"
#include "CommandBufferGL.hpp"
#include "GraphicsLayerGL.hpp"

namespace tur::gl
{
	CommandBufferGL::CommandBufferGL(IGraphicsLayer* graphicsLayer)
		: r_GraphicsLayer(static_cast<GraphicsLayerGL*>(graphicsLayer))
	{
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);
	}
	
	void CommandBufferGL::Begin()
	{
		ClearCommandState();
	}
	void CommandBufferGL::End()
	{
	}

	void CommandBufferGL::BeginRenderPass(renderpass_handle handle)
	{
		m_Commands.push_back([&, handle]() {
			if (handle == invalid_handle)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				return;
			}

			auto framebufferHandle = r_GraphicsLayer->GetRenderpass(handle);
			glBindFramebuffer(GL_FRAMEBUFFER, r_GraphicsLayer->GetFramebuffer(framebufferHandle));
		});
	}
	void CommandBufferGL::EndRenderPass()
	{
		m_Commands.push_back([]() {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		});
	}

	void CommandBufferGL::SetViewport(const Rect2D& rect)
	{
		m_Commands.push_back([rect]() {
			glViewport(rect.x, rect.y, rect.width, rect.height);
		});
	}

	void CommandBufferGL::Clear(ClearFlags flags, const ClearValue& clearValue)
	{
		m_Commands.push_back([flags, clearValue]() {
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
		});
	}

	void CommandBufferGL::BindPipeline(pipeline_handle handle)
	{
		m_Commands.push_back([&, handle]() {
			m_PipelineDescriptor = r_GraphicsLayer->GetPipelineState(handle);
			m_ActivePipeline = r_GraphicsLayer->GetPipeline(handle);

			glUseProgram(m_ActivePipeline);

			glFrontFace(GetFrontFace(m_PipelineDescriptor.frontFace));
			glPolygonMode(GL_FRONT_AND_BACK, GetPolygonMode(m_PipelineDescriptor.polygonMode));
			glCullFace(GetCullMode(m_PipelineDescriptor.cullMode));

			BindPipelineAttributes();
		});
	}
	void CommandBufferGL::BindVertexBuffer(buffer_handle handle, uint32_t binding)
	{
		m_Commands.push_back([&, handle, binding]() {
			auto bufferID = r_GraphicsLayer->GetBuffer(handle);
			m_BufferBindings[binding] = bufferID;
		});
	}

	void CommandBufferGL::BindIndexBuffer(buffer_handle handle)
	{
		m_Commands.push_back([&, handle]() {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r_GraphicsLayer->GetBuffer(handle));
		});
	}

	void CommandBufferGL::Draw(uint32_t first, uint32_t vertexCount)
	{
		m_Commands.push_back([&, first, vertexCount]() {
			glDrawArrays(GetPrimitiveTopology(m_PipelineDescriptor.primitiveTopology), first, vertexCount);
		});
	}
	void CommandBufferGL::DrawInstanced(uint32_t first, uint32_t vertexCount, uint32_t instanceCount)
	{
		m_Commands.push_back([&, first, vertexCount, instanceCount]() {
			glDrawArraysInstanced(
				GetPrimitiveTopology(m_PipelineDescriptor.primitiveTopology),
				first,
				vertexCount,
				instanceCount
			);
		});
	}
	void CommandBufferGL::DrawIndexed(uint32_t indexCount, uint32_t instanceCount)
	{
		m_Commands.push_back([&, indexCount, instanceCount]() {
			glDrawElementsInstanced(
				GetPrimitiveTopology(m_PipelineDescriptor.primitiveTopology),
				indexCount,
				GL_UNSIGNED_INT,
				nullptr,
				instanceCount
			);
		});
	}
	void CommandBufferGL::DrawIndexedInstanced(uint32_t indexCount, uint32_t instanceCount)
	{
		m_Commands.push_back([&, indexCount, instanceCount]() {
			glDrawElementsInstanced(
				GetPrimitiveTopology(m_PipelineDescriptor.primitiveTopology),
				indexCount,
				GL_UNSIGNED_INT,
				nullptr,
				instanceCount
			);
		});
	}

	void CommandBufferGL::Submit()
	{
		for (const auto& command : m_Commands)
			command();
	}

	void CommandBufferGL::BindPipelineAttributes()
	{
		auto descriptor = m_PipelineDescriptor;

		// Vertex Layout:
		size_t biggestBinding = 0;
		{
			// Fetching the biggest binding:
			for (const auto& attribute : descriptor.vertexFormat.attributes)
			{
				if (attribute.binding > biggestBinding)
					biggestBinding = attribute.binding;
			}
		}

		// assuming the user is not stupid, he'll write bindings starting at zero and counting up
		// TODO: account for user stupidity.
		std::vector<size_t> offsets(biggestBinding + 1, 0), strides(biggestBinding + 1, 0);
		{
			for (const auto& attribute : descriptor.vertexFormat.attributes)
			{
				auto typeInfo = GetDataType(attribute.type);
				strides[attribute.binding] += attribute.size * GetDataTypeSingleSize(attribute.type) * typeInfo.componentSize;
			}
		}

		uint32_t boundBuffer = invalid_handle;
		for (const auto& attribute : descriptor.vertexFormat.attributes)
		{
			glEnableVertexAttribArray(attribute.location);

			auto bufferHandle = m_BufferBindings[attribute.binding];
			glBindBuffer(GL_ARRAY_BUFFER, bufferHandle);

			auto typeInfo = GetDataType(attribute.type);
			auto attributeSize = attribute.size * typeInfo.componentSize;

			glVertexAttribPointer(
				attribute.location,
				attributeSize,
				typeInfo.format,
				attribute.normalized,
				strides[static_cast<size_t>(attribute.binding)],
				(void*)offsets[attribute.binding]
			);

			glVertexAttribDivisor(attribute.location, attribute.divisor);

			offsets[attribute.binding] += attribute.size * GetDataTypeSingleSize(attribute.type) * typeInfo.componentSize;
		}
	}

	void CommandBufferGL::BindUniforms()
	{
		
	}

	void CommandBufferGL::ClearCommandState()
	{
		m_Commands.clear();
		m_BufferBindings.clear();
		m_PipelineDescriptor = {};
	}
}