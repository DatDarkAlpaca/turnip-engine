#pragma once
#include "Common.h"
#include "GraphicsAPI.h"

#include "Rendering/Resource/Resources.h"
#include "Rendering/RenderDevice.h"
#include "Rendering/RenderContext.h"
#include "Platform/Platform.h"

namespace tur
{
	class RenderingSystem : public Singleton<RenderingSystem>
	{
	public:
		void Configure(Window& window, const GraphicsSpecification& specification);

		void DisplayVersion();

	public:
		static inline GraphicsAPI API() { return s_API; }

		tur_unique<RenderDevice>& Device() { return m_RenderDevice; }

		tur_unique<GraphicsRenderContext>& GraphicsContext() { return m_GraphicsContext; }

	private:
		static inline GraphicsAPI s_API = GraphicsAPI::NONE;
		tur_unique<RenderDevice> m_RenderDevice = nullptr;
		tur_unique<GraphicsRenderContext> m_GraphicsContext = nullptr;
	};
}