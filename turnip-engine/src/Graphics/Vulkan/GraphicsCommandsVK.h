#pragma once
#include "Common.h"

#include "Rendering/GraphicsCommands.h"
#include "Converters.h"

namespace tur::vulkan
{
	class RenderDeviceVK;

	class GraphicsRenderCommandsVK : public GraphicsRenderCommands
	{
	public:
		explicit GraphicsRenderCommandsVK(NON_OWNING RenderDeviceVK* renderContext)
			: r_RenderContext(renderContext)
		{

		}

	public:
		void Begin() override
		{
			
		}

		void SetRenderpass(RenderpassHandle handle) override { }

		void SetClearColor(const glm::vec4& color) override
		{
			
		}

		void Clear() override
		{
			
		}

		void BindPipeline(PipelineStateHandle handle) override
		{
			
		}

		void SetVertexBuffer(BufferHandle handle) override
		{
			
		}

		void SetIndexBuffer(BufferHandle handle) override
		{
			
		}

		void Draw(uint32_t first, uint32_t count) override
		{
			
		}

		void DrawIndexed(uint32_t count) override
		{
			
		}

		void End() override
		{

		}

	private:
		NON_OWNING RenderDeviceVK* r_RenderContext = nullptr;
	};
}