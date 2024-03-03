#include "pch.h"
#include "GraphicsCommandsGL.h"
#include "RenderDeviceGL.h"

namespace tur::gl
{
	GraphicsRenderCommandsGL::GraphicsRenderCommandsGL(NON_OWNING RenderDeviceGL* renderContext)
		: r_RenderDevice(renderContext)
	{
		glGenVertexArrays(1, &VAO);
	}

	void GraphicsRenderCommandsGL::Begin()
	{
		glBindVertexArray(VAO);
	}

	void GraphicsRenderCommandsGL::BeginRenderpass(RenderpassHandle handle)
	{
		auto renderpass = r_RenderDevice->GetRenderpass(handle);

		glViewport(
			renderpass.offset.x, 
			renderpass.offset.y,
			renderpass.extent.width, 
			renderpass.extent.height
		);

		glBindFramebuffer(GL_FRAMEBUFFER, renderpass.framebufferID);
	}

	void GraphicsRenderCommandsGL::EndRenderpass()
	{
		/* Implement */
	}

	void GraphicsRenderCommandsGL::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void GraphicsRenderCommandsGL::Clear(ClearFlags flags)
	{
		glClear(gl::GetClearFlag(flags));
	}

	void GraphicsRenderCommandsGL::BindPipeline(PipelineStateHandle handle)
	{
		auto [pipeline, id] = r_RenderDevice->GetPipeline(handle);
		glUseProgram(id);

		if (m_CurrentPipelineID == handle)
			return;

		m_CurrentPipelineID = handle;
		m_PrivitiveTopology = pipeline.primitiveTopology;

		// Input Layout:
		uint64_t stride = 0;
		for (const auto& attribute : pipeline.vertexFormat.attributes)
		{
			auto formatInfo = gl::GetFormat(attribute.layoutFormat);
			stride += formatInfo.componentSize * formatInfo.componentAmount;
		}

		uint64_t offset = 0;
		for (const auto& attribute : pipeline.vertexFormat.attributes)
		{
			auto formatInfo = gl::GetFormat(attribute.layoutFormat);

			glEnableVertexAttribArray(attribute.location);
			glVertexAttribPointer(
				attribute.location,
				formatInfo.componentAmount,
				gl::GetAttributeFormat(attribute.layoutFormat),
				false,
				(int)stride,
				(void*)offset
			);

			offset += formatInfo.componentSize * formatInfo.componentAmount;
		}

		// Rasterizer:
		{
			uint32_t polygonMode = gl::GetPolygonMode(pipeline.polygonMode);
			uint32_t cullFace = gl::GetCullMode(pipeline.cullMode);
			uint32_t frontFace = gl::GetFrontFace(pipeline.frontFace);

			glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

			if (pipeline.cullMode != CullMode::NONE)
			{
				glEnable(GL_CULL_FACE);
				glCullFace(cullFace);
				glFrontFace(frontFace);
			}
		}

		// Color Blending:
		{
			if (pipeline.enableColorBlending)
			{
				glEnable(GL_BLEND);
				glBlendFuncSeparate(
					gl::GetBlendFactor(pipeline.srcColorBlendFactor, BlendFactor::ONE),
					gl::GetBlendFactor(pipeline.dstColorBlendFactor, BlendFactor::ZERO),
					gl::GetBlendFactor(pipeline.srcAlphaColorBlendFactor, BlendFactor::ONE),
					gl::GetBlendFactor(pipeline.dstAlphaColorBlendFactor, BlendFactor::ZERO)
				);

				glBlendEquationSeparate(
					gl::GetBlendOperation(pipeline.colorBlendOp),
					gl::GetBlendOperation(pipeline.alphaBlendOp)
				);
			}

			if (pipeline.enableLogicOp)
			{
				glEnable(GL_COLOR_LOGIC_OP);
				glLogicOp(gl::GetLogicOp(pipeline.logicOperation));
			}
		}
	}

	void GraphicsRenderCommandsGL::SetVertexBuffer(BufferHandle handle)
	{
		auto [target, id] = r_RenderDevice->GetBuffer(handle);
		glBindBuffer(gl::GetBufferUsageFlags(target), id);
	}

	void GraphicsRenderCommandsGL::SetIndexBuffer(BufferHandle handle)
	{
		auto [target, id] = r_RenderDevice->GetBuffer(handle);
		glBindBuffer(gl::GetBufferUsageFlags(target), id);
	}

	void GraphicsRenderCommandsGL::Draw(uint32_t first, uint32_t count)
	{
		glDrawArrays(gl::GetPrimitiveTopology(m_PrivitiveTopology), first, count);
	}

	void GraphicsRenderCommandsGL::DrawIndexed(uint32_t count)
	{
		glDrawElements(gl::GetPrimitiveTopology(m_PrivitiveTopology), count, GL_UNSIGNED_INT, nullptr);
	}

	void GraphicsRenderCommandsGL::End()
	{
		glBindVertexArray(0);
	}
}
