#pragma once
#include <glm/glm.hpp>
#include "Rendering/RenderCommands.h"
#include "Resource/Resources.h"

namespace tur
{
	class GraphicsRenderCommands : public RenderCommands
	{
	public:
		virtual ~GraphicsRenderCommands() = default;

	public:
		virtual void BeginRenderpass(RenderpassHandle handle = RenderpassHandle::SCREEN_PASS) = 0;

		virtual void EndRenderpass() = 0;

		virtual void SetClearColor(const glm::vec4& color) = 0;

		virtual void Clear(ClearFlags flags) = 0;

		virtual void BindPipeline(PipelineStateHandle handle) = 0;

		virtual void SetVertexBuffer(BufferHandle handle) = 0;

		virtual void SetIndexBuffer(BufferHandle handle) = 0;

		virtual void Draw(uint32_t first, uint32_t count) = 0;

		virtual void DrawIndexed(uint32_t count) = 0;
	};
}