#pragma once

namespace tur
{
	class IGraphicsAPI_Loader
	{
	public:
		virtual ~IGraphicsAPI_Loader() = default;

	public:
		virtual void PreInitialize() { }

		virtual void PostInitialize(Window* window) { }

	protected:
		Window* window = nullptr;
	};
}