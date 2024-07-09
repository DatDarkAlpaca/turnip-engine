#pragma once
#include "Common.h"

#include "Core/View/ViewSystem.h"
#include "Core/Event/Event.h"
#include "Platform/Platform.h"

#include "Rendering/RenderDevice.h"
#include "Rendering/GraphicsCommands.h"
#include "Rendering/RenderInitializer.h"

namespace tur
{
	class TurnipEngine
	{
		friend class GraphicsEngineWrapper;

	public:
		// TODO: engine configuration
		TurnipEngine(const std::filesystem::path& configFilePath = "engine-config.json");

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
		void ConfigureRenderer(const GraphicsSpecification& specification, RenderInitializer* initializer = nullptr);

	public:
		ViewSystem& View() { return g_ViewSystem; }

		tur_unique<RenderDevice>& Device() { return g_RenderDevice; }

		Window& GetWindow() { return g_Window; }

	private:
		LoggerSystem g_LoggerSystem;
		ViewSystem g_ViewSystem;
		Window g_Window;
		tur_unique<RenderDevice> g_RenderDevice;

	private:
		bool m_Initialized = false;
	};
}