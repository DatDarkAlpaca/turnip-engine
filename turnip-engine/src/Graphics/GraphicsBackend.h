#pragma once
#include "BackendVersion.h"
#include "BackendType.h"
#include "Platform/Platform.h"

#include "Shader.h"

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
		virtual Shader* CreateShader(const ShaderDescriptor&) = 0;
	};

	tur_shared<IGraphicsBackend> MakeGraphicsBackend(BackendType type, const BackendProperties& properties);
}