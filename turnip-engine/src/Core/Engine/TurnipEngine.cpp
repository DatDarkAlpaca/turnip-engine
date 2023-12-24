#include "pch.h"
#include "TurnipEngine.h"

#include "Graphics/Graphics.h"
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
		g_ViewSystem = new ViewSystem();

		// Graphics system:
		g_GraphicsSystem = new GraphicsSystem;
		g_GraphicsSystem->Get().Initialize(BIND(&TurnipEngine::OnEvent, this));

		m_Initialized = true;
	}

	void TurnipEngine::Run()
	{
		OnEngineInitialize();

		auto& window = Graphics().GetWindow();
		window->Show();

		if (!m_Initialized)
			TUR_LOG_CRITICAL("Failed to initialize the required systems.");

		while (window->IsOpen())
		{
			window->PollEvents();

			OnUpdate();

			OnRender();

			OnRenderGUI();
		}

		window->Shutdown();
	}

	void TurnipEngine::OnEngineInitialize()
	{
		for (const auto& view : g_ViewSystem->Get())
			view->OnEngineInitialize();
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
}