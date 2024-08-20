#pragma once
#include "Core/Handle/HandleArray.hpp"
#include "Util/File.hpp"
#include "CommonGL.hpp"
#include "Platform/Platform.hpp"

#include "Graphics/GraphicsLayer.hpp"
#include "SwapchainGL.hpp"
#include "CommandBufferGL.hpp"
#include "ShaderGL.hpp"
#include "PipelineGL.hpp"
#include "BufferGL.hpp"
#include "TextureGL.hpp"
#include "FramebufferGL.hpp"

namespace tur::gl
{
	class GraphicsLayerGL final : public IGraphicsLayer
	{
	public:
		explicit GraphicsLayerGL(platform::Window& window)
			: r_Window(window)
		{

		}

	public:
		void Initialize() override 
		{

		};

		void Shutdown() override
		{

		};

	public:
		tur_unique<ISwapchain> CreateSwapChain(const SwapchainDescriptor& descriptor) override;
		tur_unique<ICommandBuffer> CreateCommandBuffer() override;

	public:
		shader_handle CreateShader(const ShaderDescriptor& descriptor) override;
		void DestroyShader(shader_handle handle) override;

	public:
		pipeline_handle CreatePipeline(const PipelineDescriptor& descriptor) override;
		virtual void DestroyPipeline(pipeline_handle handle) override;

	public:
		buffer_handle CreateBuffer(const BufferDescriptor& descriptor, const void* data) override;
		void DestroyBuffer(buffer_handle handle) override;

	public:
		texture_handle CreateTexture(const TextureDescriptor& descriptor) override;
		void DestroyTexture(texture_handle handle) override;

	public:
		framebuffer_handle CreateFramebuffer(const FramebufferDescriptor& descriptor) override;
		void DestroyFramebuffer(framebuffer_handle handle) override;

		renderpass_handle CreateRenderpass(const RenderpassDescriptor& descriptor) override;
		void DestroyRenderpass(renderpass_handle handle) override;

	public:
		gl_handle GetShader(shader_handle handle)						const { return m_Shaders.get(handle); }
		gl_handle GetPipeline(pipeline_handle handle)					const { return m_Pipelines.get(handle); }
		PipelineDescriptor GetPipelineState(pipeline_handle handle)		const { return m_PipelineStates.get(handle); }
		gl_handle GetBuffer(buffer_handle handle)						const { return m_Buffers.get(handle); }
		gl_handle GetTexture(texture_handle handle)						const { return m_Textures.get(handle); }
		gl_handle GetFramebuffer(framebuffer_handle handle)				const { return m_Framebuffers.get(handle); }
		framebuffer_handle GetRenderpass(renderpass_handle handle)		const { return m_Renderpasses.get(handle); }

	private:
		platform::Window& r_Window;

		HandleArray<gl_handle> m_Shaders;
		HandleArray<gl_handle> m_Pipelines;
		HandleArray<PipelineDescriptor> m_PipelineStates;
		HandleArray<gl_handle> m_Buffers;
		HandleArray<gl_handle> m_Textures;
		HandleArray<gl_handle> m_Framebuffers;
		HandleArray<framebuffer_handle> m_Renderpasses;
	};
}