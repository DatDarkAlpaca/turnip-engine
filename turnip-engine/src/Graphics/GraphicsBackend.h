#pragma once
#include "BackendVersion.h"
#include "BackendType.h"
#include "Platform/Platform.h"

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
		virtual void InitializeWindow(tur_unique<Window>& window) = 0;

		virtual void Present() = 0;
	};

	tur_shared<IGraphicsBackend> MakeGraphicsBackend(BackendType type, const BackendProperties& properties);
}