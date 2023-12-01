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

		// Default Window & Graphics API:
		SwitchGraphicsAPI(BackendType::OPENGL, {});

		// Default View holder:
		m_Data.viewHolder = tur::MakeUnique<ViewHolder>();

		m_Data.initialized = true;
	}

	void TurnipEngine::SwitchGraphicsAPI(BackendType type, const BackendProperties& properties)
	{
		switch (type)
		{
			case BackendType::OPENGL:
			{
				m_Data.backend = tur::MakeUnique<BackendOpenGL>(m_Data.window);
				m_Data.backend->Initialize(properties);
				m_Data.window->SetEventCallback(BIND(&TurnipEngine::OnEvent, this));
			} break;

			default:
				TUR_LOG_CRITICAL("Unsupported graphics API");
		}
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
