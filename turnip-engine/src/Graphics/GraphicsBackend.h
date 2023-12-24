#pragma once
#include "BackendType.h"
#include "Platform/Platform.h"

#include "Shader.h"
#include "Pipeline.h"
#include "Renderpass.h"
#include "CommandQueue.h"

namespace tur
{
	static constexpr inline uint32_t InvalidBackendVersion = 0xFFFFFFFF;

	struct BackendProperties
	{
		uint32_t major = InvalidBackendVersion;
		uint32_t minor = InvalidBackendVersion;
		uint32_t patch = InvalidBackendVersion;
		uint32_t extra = InvalidBackendVersion;

		std::string applicationName = "";
	};

	class IGraphicsBackend
	{
	public:
		virtual void FinishSetup(tur_unique<Window>&) { }

		virtual void Present() = 0;

	public:
		virtual tur_unique<Shader> CreateShader(const ShaderDescriptor&) = 0;

		virtual tur_unique<Renderpass> CreateRenderpass(const RenderpassDescriptor& descriptor) = 0;

		virtual tur_unique<Pipeline> CreatePipeline(const PipelineDescriptor&) = 0;
	};

	class IGraphicsBackendInitializer
	{
	public:
		virtual ~IGraphicsBackendInitializer() = default;
	};

	tur_shared<IGraphicsBackend> MakeGraphicsBackend(BackendType type, const BackendProperties& properties);
}