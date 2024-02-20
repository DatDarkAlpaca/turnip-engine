#pragma once
#include "Resource/Resources.h"
#include "Platform/Platform.h"
#include "RenderCommands.h"
#include "RenderInitializer.h"
#include "GraphicsCommands.h"

namespace tur
{
	class RenderDevice
	{
	public:
		static tur_unique<RenderDevice> Create(Window& window, const GraphicsSpecification& specification, RenderInitializer* initializer);

		virtual ~RenderDevice() = default;

	public:
		virtual RenderpassHandle CreateRenderpass(const RenderpassDescriptor& renderpassDescription) = 0;

		virtual BufferHandle CreateBuffer(const BufferDescriptor& bufferDescription) = 0;

		virtual ShaderHandle CreateShader(const ShaderDescriptor& shaderDescriptor) = 0;

		virtual PipelineStateHandle CreatePipeline(const PipelineStateDescriptor& pipelineDescriptor) = 0;

	public:
		virtual tur_unique<GraphicsRenderCommands> CreateGraphicsCommands() { TUR_LOG_ERROR("This API does not support graphics commands."); return nullptr; }

	public:
		virtual Barrier Submit(RenderCommands* context) { return {}; /* TODO: CommandList */ }

		virtual void WaitBarrier(const Barrier& barrier) { /* TODO: CommandList */ }

		virtual void Present() = 0;

	public:
		virtual void DisplayVersion() { TUR_LOG_INFO("DisplayVersion() is not implemented in this graphics API."); }
	};
}