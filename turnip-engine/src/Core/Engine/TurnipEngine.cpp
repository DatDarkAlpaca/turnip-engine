#include "pch.h"
#include "TurnipEngine.h"

#include "Graphics/Graphics.h"
#include "Platform/Platform.h"

namespace tur
{
	TurnipEngine::TurnipEngine()
	{
		tur::platform::Setup();

		g_LoggerSystem = new LoggerSystem();
		g_LoggerSystem->Get().Initialize();

		// Default View holder:
		g_ViewSystem = new ViewSystem();

		m_Initialized = true;
	}

	tur_shared<IGraphicsBackend> TurnipEngine::CreateGraphicsAPI(BackendType type, const BackendProperties& properties)
	{
		tur_shared<IGraphicsBackend> graphicsBackend;
		graphicsBackend = MakeGraphicsBackend(type, properties);
		graphicsBackend->InitializeWindow(m_Data.window);

		m_Data.window->SetEventCallback(BIND(&TurnipEngine::OnEvent, this));
		
		return graphicsBackend;
	}

	void TurnipEngine::CreateWindow(const WindowProperties& properties)
	{
		m_Data.window = tur::MakeUnique<Window>();
		m_Data.window->Initialize(properties);
		m_Data.window->SetEventCallback(BIND(&TurnipEngine::OnEvent, this));
	}

	void TurnipEngine::AddView(tur::tur_unique<View> view)
	{
		view->SetHandler(g_ViewSystem);
		g_ViewSystem->AddView(std::move(view));
	}

	void TurnipEngine::Run()
	{
		TUR_ASSERT(m_Data.window, "The application doesn't have a window. Create one using CreateWindow() or CreateGraphicsAPI()");
	
		OnEngineInitialize();

		auto& window = m_Data.window;
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
	}

	void TurnipEngine::OnEngineInitialize()
	{
		for (const auto& view : *g_ViewSystem)
			view->OnEngineInitialize();
	}

	void TurnipEngine::OnRender()
	{
		for (const auto& view : *g_ViewSystem)
			view->OnRender();
	}

	void TurnipEngine::OnRenderGUI()
	{
		for (const auto& view : *g_ViewSystem)
			view->OnRenderGUI();
	}

	void TurnipEngine::OnUpdate()
	{
		for (const auto& view : *g_ViewSystem)
			view->OnUpdate();
	}

	void TurnipEngine::OnEvent(Event& event)
	{
		for (const auto& view : *g_ViewSystem)
			view->OnEvent(event);
	}
}