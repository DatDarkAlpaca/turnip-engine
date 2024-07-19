#pragma once
#include "Common.h"

#include "Core/Assets/AssetLibrary.hpp"
#include "Core/Window/WindowSystem.hpp"
#include "Core/Worker/WorkerPool.hpp"
#include "Core/View/ViewSystem.h"

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
		AssetLibrary& GetAssetLibrary() { return g_AssetLibrary; }

		ViewSystem& GetViewSystem() { return g_ViewSystem; }

		WindowSystem& GetWindowSystem() { return g_WindowSystem; }

		WorkerPool& GetWorkerPool() { return g_WorkerPool; }

	private:
		LoggerSystem g_LoggerSystem;
		AssetLibrary g_AssetLibrary;
		WindowSystem g_WindowSystem;
		ViewSystem g_ViewSystem;
		WorkerPool g_WorkerPool;

	private:
		bool m_Initialized = false;
	};
}