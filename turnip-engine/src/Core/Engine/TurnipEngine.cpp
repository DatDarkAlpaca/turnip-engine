#include "pch.hpp"
#include "TurnipEngine.hpp"
#include "Platform/Platform.hpp"
#include "Core/Config/ConfigSystem.hpp"

namespace tur
{
	TurnipEngine::TurnipEngine(const std::filesystem::path& configFilePath)
	{
		ConfigSystem configSystem(configFilePath);

		platform::InitializePlatform();

		// Logger System:
		g_LoggerSystem.Get().Initialize();

		// Window System:
		g_WindowSystem.Initialize(configSystem);
		g_WindowSystem.SetEventCallback(BIND(&TurnipEngine::OnEvent, this));

		m_Initialized = true;
	}

	void TurnipEngine::Run()
	{
		Initialize();

		if (!m_Initialized)
			TUR_LOG_CRITICAL("Failed to initialize the required systems.");

		while (g_WindowSystem.GetWindow().IsOpen())
		{
			g_WindowSystem.GetWindow().PollEvents();

			g_WorkerPool.PollTasks();

			OnUpdate();

			OnRender();

			OnRenderGUI();
		}

		Shutdown();
	}

	void TurnipEngine::Initialize()
	{	
		OnEngineStartup();
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