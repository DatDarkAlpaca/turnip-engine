#include "pch.h"
#include "TurnipEngine.h"

#include "Platform/Platform.h"

namespace tur
{
	TurnipEngine::TurnipEngine(const WindowProperties& properties)
	{
		tur::platform::Setup();

		// Logger:
		g_LoggerSystem.Get().Initialize();

		// Window:
		g_Window.Initialize(properties);
		g_Window.SetEventCallback(BIND(&TurnipEngine::OnEvent, this));
		
		// Default rendering system:
		if (g_RenderingSystem.API().api == GraphicsAPI::NONE)
			g_RenderingSystem.Configure(g_Window, { GraphicsAPI::OPENGL, 3, 3 });

		m_Initialized = true;
	}

	void TurnipEngine::Run()
	{
		Initialize();

		if (!m_Initialized)
			TUR_LOG_CRITICAL("Failed to initialize the required systems.");

		while (g_Window.IsOpen())
		{
			g_Window.PollEvents();

			OnUpdate();

			OnRender();

			OnRenderGUI();
		}

		Shutdown();
	}

	void TurnipEngine::Initialize()
	{
		// ImGUI View:
		g_ViewSystem.Add(tur::MakeUnique<ViewImGUI>(g_Window));
		
		// Engine Startup:
		OnEngineStartup();
		g_Window.Show();
	}

	void TurnipEngine::Shutdown()
	{
		OnEngineShutdown();

		g_Window.Shutdown();
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

	void TurnipEngine::ConfigureRenderer(const GraphicsSpecification& specification)
	{
		g_RenderingSystem.Configure(g_Window, specification);
	}
}