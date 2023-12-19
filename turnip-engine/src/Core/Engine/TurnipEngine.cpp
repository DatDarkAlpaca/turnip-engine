#include "pch.h"
#include "TurnipEngine.h"

#include "Graphics/Graphics.h"
#include "Platform/Platform.h"

namespace tur
{
	TurnipEngine::TurnipEngine()
	{
		tur::platform::Setup();

		tur::InitializeLogger();

		// Default View holder:
		m_Data.viewHolder = tur::MakeUnique<ViewHolder>();

		m_Data.initialized = true;
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
		view->SetHandler(m_Data.viewHolder.get());
		m_Data.viewHolder->AddView(std::move(view));
	}

	void TurnipEngine::Run()
	{
		TUR_ASSERT(m_Data.window, "The application doesn't have a window. Create one using CreateWindow() or CreateGraphicsAPI()");
	
		OnEngineInitialize();

		auto& window = m_Data.window;
		window->Show();

		if (!m_Data.initialized)
			TUR_LOG_CRITICAL("Failed to initialize the required subsystems.");

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
		for (const auto& view : *m_Data.viewHolder)
			view->OnEngineInitialize();
	}

	void TurnipEngine::OnRender()
	{
		for (const auto& view : *m_Data.viewHolder)
			view->OnRender();
	}

	void TurnipEngine::OnRenderGUI()
	{
		for (const auto& view : *m_Data.viewHolder)
			view->OnRenderGUI();
	}

	void TurnipEngine::OnUpdate()
	{
		for (const auto& view : *m_Data.viewHolder)
			view->OnUpdate();
	}

	void TurnipEngine::OnEvent(Event& event)
	{
		for (const auto& view : *m_Data.viewHolder)
			view->OnEvent(event);
	}
}
