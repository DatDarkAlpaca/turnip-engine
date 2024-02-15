#pragma once
#include "Common.h"

#include "Core/View/ViewSystem.h"
#include "Core/Event/Event.h"
#include "Platform/Platform.h"
#include "Rendering/RenderingSystem.h"

namespace tur
{
	class TurnipEngine
	{
		friend class GraphicsEngineWrapper;

	public:
		// TODO: engine configuration
		TurnipEngine(const WindowProperties& properties = {});

	public:
		void Run();

	private:
		void Initialize();

		void Shutdown();

	private:
		void OnEngineStartup();

		void OnRender();

		void OnRenderGUI();

		void OnUpdate();

		void OnEvent(Event& event);

		void OnEngineShutdown();
		
	public:
		void ConfigureRenderer(const GraphicsSpecification& specification);

	public:
		ViewSystem& View() { return g_ViewSystem.Get(); }

		RenderingSystem& Renderer() { return g_RenderingSystem.Get(); }

		tur_unique<RenderDevice>& Device() { return g_RenderingSystem.Get().Device(); }

		Window& GetWindow() { return g_Window; }

	private:
		LoggerSystem g_LoggerSystem;
		ViewSystem g_ViewSystem;
		RenderingSystem g_RenderingSystem;
		Window g_Window;

	private:
		bool m_Initialized = false;
	};
}