#pragma once
#include <glm/glm.hpp>
#include "Rendering/Context.h"
#include "Resource/Resources.h"

namespace tur
{
	class GraphicsRenderContext : public Context
	{
	public:
		virtual ~GraphicsRenderContext() = default;

	public:
		virtual void SetClearColor(const glm::vec4& color) = 0;

		virtual void Clear() = 0;

		virtual void SetPipeline(PipelineStateHandle handle) = 0;

		virtual void SetVertexBuffer(BufferHandle handle) = 0;

		virtual void SetIndexBuffer(BufferHandle handle) = 0;

		virtual void Draw(uint32_t first, uint32_t count) = 0;

		virtual void DrawIndexed(uint32_t count) = 0;
	};
}