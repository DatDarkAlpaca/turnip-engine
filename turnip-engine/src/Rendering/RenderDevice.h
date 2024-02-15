#pragma once
#include "Resource/Resources.h"

namespace tur
{
	class RenderDevice
	{
	public:
		virtual ~RenderDevice() = default;

	public:
		virtual BufferHandle CreateBuffer(const BufferDescriptor& bufferDescription) = 0;

		virtual ShaderHandle CreateShader(const ShaderDescriptor& shaderDescriptor) = 0;

		virtual PipelineStateHandle CreatePipeline(const PipelineStateDescriptor& pipelineDescriptor) = 0;

	public:
		virtual void Present() = 0;
	};
}