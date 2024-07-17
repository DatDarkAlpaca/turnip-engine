#pragma once
#include "Common.h"

#include "Core/Assets/AssetLibrary.hpp"
#include "Core/View/ViewSystem.h"
#include "Core/Event/Event.h"

#include "Rendering/RenderDevice.h"
#include "Rendering/GraphicsCommands.h"
#include "Rendering/RenderInitializer.h"

#include "Platform/Platform.h"

namespace tur
{
	class TurnipEngine
	{
		friend class GraphicsEngineWrapper;

	public:
		TurnipEngine(const std::filesystem::path& configFilePath = "engine-config.json");

	public:
		void Run();

	private:
		void Initialize();

		void Shutdown();

	public:
		void AddView(tur_unique<View> view);

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
		AssetLibrary& GetAssetLibrary() { return g_AssetLibrary; }

		ViewSystem& GetViewSystem() { return g_ViewSystem; }

		Window& GetWindow() { return g_Window; }

		tur_unique<RenderDevice>& Device() { return g_RenderDevice; }

	private:
		LoggerSystem g_LoggerSystem;
		AssetLibrary g_AssetLibrary;
		ViewSystem g_ViewSystem;
		Window g_Window;
		
		tur_unique<RenderDevice> g_RenderDevice;

	private:
		bool m_Initialized = false;
	};
}