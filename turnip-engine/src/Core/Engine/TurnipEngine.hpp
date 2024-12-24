#pragma once
#include "Common.hpp"

#include "Core/Assets/AssetLibrary.hpp"
#include "Core/Worker/WorkerPool.hpp"
#include "Core/View/ViewSystem.hpp"
#include "Graphics/GraphicsSystem.hpp"

#include "Platform/Platform.hpp"

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

		WorkerPool& GetWorkerPool() { return g_WorkerPool; }

		GraphicsSystem& GetGraphicsSystem() { return g_GraphicsSystem; }

		platform::Window& GetWindow() { return g_Window; }

	private:
		AssetLibrary g_AssetLibrary;
		ViewSystem g_ViewSystem;
		WorkerPool g_WorkerPool;
		GraphicsSystem g_GraphicsSystem;

		platform::Window g_Window;

	private:
		bool m_Initialized = false;
	};
}