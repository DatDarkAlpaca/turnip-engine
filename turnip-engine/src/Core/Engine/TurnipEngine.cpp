#include "pch.hpp"
#include "TurnipEngine.hpp"

#include "Core/Config/ConfigSystem.hpp"
#include "Platform/Platform.hpp"

namespace tur
{
	TurnipEngine::TurnipEngine(const std::filesystem::path& configFilePath)
	{
		ConfigSystem configSystem(configFilePath);

		platform::InitializePlatform();

		// Logger System:
		g_LoggerSystem.Get().Initialize();

		// Graphics System:
		g_GraphicsSystem.Initialize(configSystem, g_WindowSystem.GetWindow());

		// Window System:
		g_WindowSystem.SetEventCallback(BIND(&TurnipEngine::OnEvent, this));

		// UI:
		g_UISystem.SetUIBackend(CreateUIBackend(configSystem, g_WindowSystem.GetWindow()));
		g_UISystem.Initialize(configSystem, g_WindowSystem.GetWindow());

		m_Initialized = true;
	}

	void TurnipEngine::Run()
	{
		OnEngineStartup();

		if (!m_Initialized)
			TUR_LOG_CRITICAL("Failed to initialize the required systems.");

		while (g_WindowSystem.GetWindow().IsOpen())
		{
			g_WindowSystem.GetWindow().PollEvents();
			g_WorkerPool.PollTasks();

			OnUpdate();

			OnRender();

			g_UISystem.StartFrame();

			OnRenderGUI();

			g_UISystem.EndFrame();

			g_WindowSystem.GetWindow().SwapBuffers();
		}

		Shutdown();
	}

	void TurnipEngine::Shutdown()
	{
		OnEngineShutdown();

		g_WindowSystem.Shutdown();
	}

	void TurnipEngine::AddView(tur_unique<View> view)
	{
		view->SetEngine(this);
		g_ViewSystem.Add(std::move(view));
	}

	void TurnipEngine::OnEngineStartup()
	{
		for (const auto& view : g_ViewSystem)
			view->OnEngineStartup();
	}

	void TurnipEngine::OnRender()
	{
		for (const auto& view : g_ViewSystem)
			view->OnRender();
	}

	void TurnipEngine::OnRenderGUI()
	{
		for (const auto& view : g_ViewSystem)
			view->OnRenderGUI();
	}

	void TurnipEngine::OnUpdate()
	{
		for (const auto& view : g_ViewSystem)
			view->OnUpdate();
	}

	void TurnipEngine::OnEvent(Event& event)
	{
		for (const auto& view : g_ViewSystem)
			view->OnEvent(event);
	}

	void TurnipEngine::OnEngineShutdown()
	{
		for (const auto& view : g_ViewSystem)
			view->OnEngineShutdown();
	}
}