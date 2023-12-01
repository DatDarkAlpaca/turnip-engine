#pragma once
#include "BackendVersion.h"

namespace tur
{
	struct BackendProperties
	{
		BackendVersion version;
	};

	class IGraphicsBackend
	{
	public:
		virtual void Initialize(const BackendProperties& properties) = 0;
	};
}