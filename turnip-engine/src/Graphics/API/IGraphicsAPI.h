#pragma once
#include "IGraphicsAPI_Loader.h"

namespace tur
{
	class IGraphicsAPI
	{
	public:
		virtual ~IGraphicsAPI() = default;

	public:
		virtual void Shutdown() = 0;
	};
}