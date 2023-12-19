#pragma once
#include "BackendVersion.h"
#include "BackendType.h"
#include "Platform/Platform.h"

#include "Shader.h"
#include "Pipeline.h"

namespace tur
{
	struct BackendProperties
	{
		std::string applicationName = "";
		BackendVersion version;
	};

	class IGraphicsBackend
	{
	public:
		virtual void InitializeWindow(tur_unique<Window>&) = 0;

		virtual void Present() = 0;

	public:
		virtual tur_unique<Shader> CreateShader(const ShaderDescriptor&) = 0;

		virtual tur_unique<Pipeline> CreatePipeline(const PipelineDescriptor&) = 0;
	};

	tur_shared<IGraphicsBackend> MakeGraphicsBackend(BackendType type, const BackendProperties& properties);
}