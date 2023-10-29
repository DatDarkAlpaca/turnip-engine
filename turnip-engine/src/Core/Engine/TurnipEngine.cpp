#include "pch.h"
#include "TurnipEngine.h"
#include "Platform/Platform.h"

namespace tur
{
	TurnipEngine::TurnipEngine()
	{
		tur::PlatformSetup();

		// Creates a default view holder:
		m_Data.viewHolder = tur::MakeUnique<ViewHolder>();

		// Creates a default window:
		RequestWindow({});

		m_Data.initialized = true;
	}

	void TurnipEngine::RequestWindow(const WindowProperties& properties)
	{
		m_Data.window = Window::Create(properties);
		m_Data.window->SetEventCallback(BIND(&TurnipEngine::OnEvent, this));

		TUR_ASSERT(m_Data.window, "Request to modify the engine's window Has failed.");
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
