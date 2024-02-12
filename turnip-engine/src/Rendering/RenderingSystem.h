#pragma once
#include "Common.h"
#include "GraphicsAPI.h"
#include "Platform/Platform.h"

namespace tur
{
	class RenderingSystem : public Singleton<RenderingSystem>
	{
	public:
		void Configure(Window& window, const GraphicsSpecification& specification);

	public:
		void DisplayVersion();

	public:
		static inline GraphicsAPI API() { return s_API; }

	private:
		static inline GraphicsAPI s_API = GraphicsAPI::NONE;
	};
}