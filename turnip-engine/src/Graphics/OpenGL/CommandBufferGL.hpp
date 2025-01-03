#pragma once
#include "Graphics/Base/CommandBuffer.hpp"
#include "Graphics/GraphicsLayer.hpp"
#include "CommonGL.hpp"

namespace tur::gl
{
	class GraphicsLayerGL;

	class CommandBufferGL : public ICommandBuffer
	{
	public:
		explicit CommandBufferGL(IGraphicsLayer* graphicsLayer);

	public:
		void Begin() override;
		void End() override;

	public:
		void BeginRenderPass(renderpass_handle handle) override;
		void EndRenderPass() override;

		void SetViewport(const Rect2D& rect) override;
		void Clear(ClearFlags flags, const ClearValue& clearValue) override;

		void BindPipeline(pipeline_handle handle) override;
		void BindVertexBuffer(buffer_handle handle, uint32_t binding) override;
		void BindIndexBuffer(buffer_handle handle) override;
		void BindTexture(texture_handle handle) override;

		void Draw(uint32_t first, uint32_t vertexCount) override;
		void DrawInstanced(uint32_t first, uint32_t vertexCount, uint32_t instanceCount) override;
		void DrawIndexed(uint32_t indexCount, uint32_t instanceCount = 1) override;
		void DrawIndexedInstanced(uint32_t indexCount, uint32_t instanceCount) override;

		void Submit() override;

	private:
		void UpdateTextureImmediate()
		{

		}

	private:
		void BindPipelineAttributes();

		void BindUniforms();

		void ClearCommandState();

	private:
		NON_OWNING GraphicsLayerGL* r_GraphicsLayer = nullptr;
		gl_handle m_VAO = invalid_handle;

		std::unordered_map<uint32_t, gl_handle> m_BufferBindings;
		std::vector<std::function<void()>> m_Commands;

		PipelineDescriptor m_PipelineDescriptor;
		gl_handle m_ActivePipeline = invalid_handle;
	};
}