#include "pch.h"
#include "TurnipEngine.h"

#include "Platform/Platform.h"

namespace tur
{
	TurnipEngine::TurnipEngine()
	{
		tur::platform::Setup();

		// Logger system:
		g_LoggerSystem = new LoggerSystem;
		g_LoggerSystem->Get().Initialize();

		// View system:
		g_ViewSystem = new ViewSystem;
		g_ViewSystem->Get();

		m_Initialized = true;
	}

	void TurnipEngine::Run()
	{
		OnEngineStartup();

		//auto& window = Graphics().GetWindow();
		//window->Show();

		if (!m_Initialized)
			TUR_LOG_CRITICAL("Failed to initialize the required systems.");

		/*while (window->IsOpen())
		{
			window->PollEvents();

			OnUpdate();

			OnRender();

			OnRenderGUI();
		}*/

		OnEngineShutdown();

		//window->Shutdown();
	}

	void TurnipEngine::OnEngineStartup()
	{
		for (const auto& view : g_ViewSystem->Get())
			view->OnEngineStartup();
	}

	void TurnipEngine::OnRender()
	{
		for (const auto& view : g_ViewSystem->Get())
			view->OnRender();
	}

	void TurnipEngine::OnRenderGUI()
	{
		for (const auto& view : g_ViewSystem->Get())
			view->OnRenderGUI();
	}

	void TurnipEngine::OnUpdate()
	{
		for (const auto& view : g_ViewSystem->Get())
			view->OnUpdate();
	}

	void TurnipEngine::OnEvent(Event& event)
	{
		for (const auto& view : g_ViewSystem->Get())
			view->OnEvent(event);
	}

	void TurnipEngine::OnEngineShutdown()
	{
		for (const auto& view : g_ViewSystem->Get())
			view->OnEngineShutdown();
	}
}