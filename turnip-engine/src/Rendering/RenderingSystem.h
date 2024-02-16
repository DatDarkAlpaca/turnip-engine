#pragma once
#include "Common.h"
#include "GraphicsAPI.h"

#include "Rendering/Resource/Resources.h"
#include "Rendering/RenderDevice.h"
#include "Rendering/GraphicsCommands.h"
#include "Platform/Platform.h"

namespace tur
{
	class RenderingSystem
	{
	public:
		void Configure(Window& window, const GraphicsSpecification& specification);

		void DisplayVersion();

	public:
		static inline GraphicsSpecification API() { return s_API; }

		tur_unique<RenderDevice>& Device() { return m_RenderDevice; }

		tur_unique<GraphicsRenderCommands>& GraphicsContext() { return m_GraphicsContext; }

	private:
		static inline GraphicsSpecification s_API = {};

		tur_unique<RenderDevice> m_RenderDevice = nullptr;
		tur_unique<GraphicsRenderCommands> m_GraphicsContext = nullptr;
	};
}