#pragma once
#include "Common.h"

#include "Rendering/RenderContext.h"
#include "RenderDeviceGL.h"
#include "Converters.h"

#include <glad/glad.h>

namespace tur
{
	class GraphicsRenderContextGL : public GraphicsRenderContext
	{
	public:
		explicit GraphicsRenderContextGL(NON_OWNING RenderDeviceGL* renderContext)
			: r_RenderContext(renderContext)
		{
			glGenVertexArrays(1, &VAO);
		}

	public:
		void Begin() override
		{
			glBindVertexArray(VAO);
		}

		void SetClearColor(const glm::vec4& color) override
		{
			glClearColor(color.r, color.g, color.b, color.a);
		}

		void Clear() override
		{
			glClear(GL_COLOR_BUFFER_BIT);
		}

		void SetPipeline(PipelineStateHandle handle) override
		{
			auto [pipeline, id] = r_RenderContext->GetPipeline(handle);
			glUseProgram(id);

			if (m_CurrentPipelineID == handle)
				return;

			m_CurrentPipelineID = handle;
			m_PrivitiveTopology = pipeline.primitiveTopology;

			// Input Layout:
			uint64_t stride = 0;
			for (const auto& layout : pipeline.inputLayouts)
				stride += layout.componentAmount * GetLayoutTypeSize(layout.valueType);
			
			uint64_t offset = 0;
			for (const auto& layout : pipeline.inputLayouts)
			{
				glEnableVertexAttribArray(layout.location);
				glVertexAttribPointer(
					layout.location,
					layout.componentAmount,
					gl::GetInputLayoutType(layout.valueType),
					false,
					(int)stride,
					(void*)offset
				);

				offset += layout.componentAmount * GetLayoutTypeSize(layout.valueType);
			}
			return;

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

		void SetVertexBuffer(BufferHandle handle) override
		{
			auto [target, id] = r_RenderContext->GetBuffer(handle);
			glBindBuffer(gl::GetBufferBindingFlag(target), id);
		}

		void SetIndexBuffer(BufferHandle handle) override
		{
			auto [target, id] = r_RenderContext->GetBuffer(handle);
			glBindBuffer(gl::GetBufferBindingFlag(target), id);
		}

		void Draw(uint32_t first, uint32_t count) override
		{
			glDrawArrays(gl::GetPrimitiveTopology(m_PrivitiveTopology), first, count);
		}

		void DrawIndexed(uint32_t count) override
		{
			glDrawElements(gl::GetPrimitiveTopology(m_PrivitiveTopology), count, GL_UNSIGNED_INT, nullptr);
		}

		void End() override
		{
			glBindVertexArray(0);
		}

	private:
		NON_OWNING RenderDeviceGL* r_RenderContext = nullptr;

	private:
		PipelineStateHandle m_CurrentPipelineID = PipelineStateHandle::INVALID;
		PrimitiveTopology m_PrivitiveTopology = PrimitiveTopology::POINTS;
		uint32_t VAO = InvalidHandle;
	};
}