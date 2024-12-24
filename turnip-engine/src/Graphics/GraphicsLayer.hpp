#pragma once
#include "CommonGraphics.hpp"

#include "Graphics/Base/Swapchain.hpp"
#include "Graphics/Base/Shader.hpp"
#include "Graphics/Base/Pipeline.hpp"
#include "Graphics/Base/Buffer.hpp"
#include "Graphics/Base/Texture.hpp"
#include "Graphics/Base/Framebuffer.hpp"
#include "Graphics/Base/Renderpass.hpp"
#include "Graphics/Base/CommandBuffer.hpp"

#define LOG_UNSUPPORTED_FUNCTION(FunctionName) \
	TUR_LOG_ERROR(std::string(FunctionName) + " is unsupported.");

namespace tur
{
	class IGraphicsLayer
	{
	public:
		explicit IGraphicsLayer() = default;
		virtual ~IGraphicsLayer() = default;

	public:
		virtual void Initialize() { };

		virtual void Shutdown() { };

	public:
		virtual tur_unique<ISwapchain> CreateSwapChain(const SwapchainDescriptor& descriptor)
		{
			LOG_UNSUPPORTED_FUNCTION("CreateSwapChain");
			return nullptr;
		}
		virtual tur_unique<ICommandBuffer> CreateCommandBuffer()
		{
			LOG_UNSUPPORTED_FUNCTION("CreateCommandBuffer");
			return nullptr;
		};

	public:
		virtual shader_handle CreateShader(const ShaderDescriptor& descriptor) 
		{
			LOG_UNSUPPORTED_FUNCTION("CreateShader");
			return invalid_handle;
		};
		virtual void DestroyShader(shader_handle handle)
		{
			LOG_UNSUPPORTED_FUNCTION("DestroyShader");
		};

	public:
		virtual pipeline_handle CreatePipeline(const PipelineDescriptor& descriptor)
		{
			LOG_UNSUPPORTED_FUNCTION("CreatePipeline");
			return invalid_handle;
		}
		virtual void DestroyPipeline(pipeline_handle handle)
		{
			LOG_UNSUPPORTED_FUNCTION("DestroyPipeline");
		}

	public:
		virtual buffer_handle CreateBuffer(const BufferDescriptor& descriptor, const void* data)
		{
			LOG_UNSUPPORTED_FUNCTION("CreateBuffer");
			return invalid_handle;
		}
		virtual void DestroyBuffer(buffer_handle handle)
		{
			LOG_UNSUPPORTED_FUNCTION("DestroyBuffer");
		}

	public:
		virtual texture_handle CreateTexture(const TextureDescriptor& descriptor, const void* data)
		{
			LOG_UNSUPPORTED_FUNCTION("CreateTexture");
			return invalid_handle;
		}
		virtual void DestroyTexture(texture_handle handle)
		{
			LOG_UNSUPPORTED_FUNCTION("DestroyTexture");
		}

	public:
		virtual framebuffer_handle CreateFramebuffer(const FramebufferDescriptor& descriptor)
		{
			LOG_UNSUPPORTED_FUNCTION("CreateFramebuffer");
			return invalid_handle;
		}
		virtual void DestroyFramebuffer(framebuffer_handle handle)
		{
			LOG_UNSUPPORTED_FUNCTION("DestroyFramebuffer");
		}

	public:
		virtual renderpass_handle CreateRenderpass(const RenderpassDescriptor& descriptor)
		{
			LOG_UNSUPPORTED_FUNCTION("CreateRenderpass");
			return invalid_handle;
		}
		virtual void DestroyRenderpass(renderpass_handle handle)
		{
			LOG_UNSUPPORTED_FUNCTION("DestroyRenderpass");
		}
	};
}