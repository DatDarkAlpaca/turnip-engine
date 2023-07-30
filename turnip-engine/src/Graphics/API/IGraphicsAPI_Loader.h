#pragma once
#include "IGraphicsAPI.h"

namespace tur
{
	class IGraphicsAPI_Loader
	{
	public:
		virtual ~IGraphicsAPI_Loader() = default;

	public:
		virtual void PreInitialize() { }

		virtual void PostInitialize(Window* window, IGraphicsAPI* api) { }

	protected:
		Window* window = nullptr;
	};
}