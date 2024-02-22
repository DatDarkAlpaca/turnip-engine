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
		explicit GraphicsRenderCommandsVK(NON_OWNING RenderDeviceVK* renderContext);

	public:
		void Begin() override;

		void BeginRenderpass(RenderpassHandle handle = RenderpassHandle::SCREEN_PASS) override;

		void EndRenderpass() override;
		
		void SetClearColor(const glm::vec4& color) override;

		void Clear(ClearFlags flags) override;

		void BindPipeline(PipelineStateHandle handle) override;

		void SetVertexBuffer(BufferHandle handle) override;

		void SetIndexBuffer(BufferHandle handle) override;

		void Draw(uint32_t first, uint32_t count) override;

		void DrawIndexed(uint32_t count) override;

		void End() override;

	public:
		inline vk::CommandBuffer GetCommandBuffer() { return m_CurrentBuffer; }

	private:
		vk::CommandBuffer m_CurrentBuffer = { };

	private:
		NON_OWNING RenderDeviceVK* r_Device = nullptr;

	private:
		glm::vec4 m_ClearColor = {};
	};
}