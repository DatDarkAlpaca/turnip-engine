#pragma once
#include "Graphics/CommonGraphics.hpp"

#include "Buffer.hpp"
#include "Pipeline.hpp"
#include "Renderpass.hpp"

namespace tur
{
	class ICommandBuffer
	{
	public:
		virtual ~ICommandBuffer() = default;

	public:
		virtual void Begin() = 0;
		virtual void End() = 0;

	public:
		virtual void BeginRenderPass(renderpass_handle handle = invalid_handle) = 0;
		virtual void EndRenderPass() = 0;

		virtual void SetViewport(const Rect2D& rect) = 0;
		virtual void Clear(ClearFlags flags, const ClearValue& clearValue = {}) = 0;

		virtual void BindPipeline(pipeline_handle handle) = 0;
		virtual void BindVertexBuffer(buffer_handle handle, uint32_t binding) = 0;
		virtual void BindIndexBuffer(buffer_handle handle) = 0;
		virtual void BindTexture(texture_handle handle) = 0 ;

		virtual void Draw(uint32_t first, uint32_t vertexCount) = 0;
		virtual void DrawInstanced(uint32_t first, uint32_t vertexCount, uint32_t instanceCount) = 0;
		virtual void DrawIndexed(uint32_t indexCount, uint32_t instanceCount = 1) = 0;
		virtual void DrawIndexedInstanced(uint32_t indexCount, uint32_t instanceCount = 1) = 0;

		virtual void Submit() = 0;
	};
}