#pragma once

namespace tur
{
	class IRenderingBackend
	{
	public:
		virtual void Initialize() = 0;
		virtual void Shutdown() = 0;
	};
}