#pragma once
#include "Common.h"

#include "Rendering/GraphicsCommands.h"
#include "Converters.h"

#include <glad/glad.h>

namespace tur::gl
{
	class RenderDeviceGL;

	class GraphicsRenderCommandsGL : public GraphicsRenderCommands
	{
	public:
		explicit GraphicsRenderCommandsGL(NON_OWNING RenderDeviceGL* renderDevice);

	public:
		void Begin() override;

		void BeginRenderpass(RenderpassHandle handle) override;
		
		void SetViewport() override;

		void SetViewport(const Viewport& viewport) override;

		void EndRenderpass() override;

		void SetClearColor(const glm::vec4& color) override;

		void Clear(ClearFlags flags) override;

		void BindPipeline(PipelineStateHandle handle) override;

		void SetVertexBuffer(BufferHandle handle) override;

		void SetIndexBuffer(BufferHandle handle) override;

		void Draw(uint32_t first, uint32_t count) override;

		void DrawIndexed(uint32_t count) override;

		void End() override;

	private:
		NON_OWNING RenderDeviceGL* r_RenderDevice = nullptr;

	private:
		PipelineStateHandle m_CurrentPipelineID = PipelineStateHandle::INVALID;
		PrimitiveTopology m_PrivitiveTopology = PrimitiveTopology::POINTS;
		uint32_t VAO = InvalidHandle;
	};
}