#include "pch.hpp"
#include "TurnipEngine.hpp"

#include "Core/Config/ConfigSystem.hpp"
#include "Platform/Platform.hpp"

#include "Core/Config/JSONConfigParser.hpp"

namespace tur
{
	TurnipEngine::TurnipEngine(const std::filesystem::path& configFilePath)
	{
		platform::InitializePlatform();

		// Config System:
		ConfigSystem configSystem(configFilePath);
		configSystem.Initialize(tur::MakeUnique<JSONConfigParser>());

		// Logger System:
		g_LoggerSystem.Get().Initialize();

		// Graphics System:
		g_GraphicsSystem.Initialize(configSystem.data(), g_Window);
		g_Window.SetEventCallback(BIND(&TurnipEngine::OnEvent, this));

		// UI:
		// g_UISystem.SetUIBackend(CreateUIBackend(configSystem.data(), g_WindowSystem.GetWindow()));
		// g_UISystem.Initialize(configSystem.data(), g_WindowSystem.GetWindow());

		m_Initialized = true;
	}

	void TurnipEngine::Run()
	{
		OnEngineStartup();

		if (!m_Initialized)
			TUR_LOG_CRITICAL("Failed to initialize the required systems.");

		while (g_Window.IsOpen())
		{
			g_Window.PollEvents();
			g_WorkerPool.PollTasks();

			OnUpdate();

			OnRender();

			// g_UISystem.StartFrame();

			OnRenderGUI();

			// g_UISystem.EndFrame();
		}

		Shutdown();
	}

	void TurnipEngine::Shutdown()
	{
		OnEngineShutdown();

		g_Window.Shutdown();
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