#pragma once

namespace tur
{
	class IGraphicsAPI
	{
	public:
		virtual ~IGraphicsAPI() = default;

	public:
		virtual void SwapBuffers() = 0;
	};
}