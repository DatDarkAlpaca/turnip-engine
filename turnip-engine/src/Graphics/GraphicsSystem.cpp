#include "pch.h"
#include "GraphicsSystem.h"

namespace tur
{
	void GraphicsSystem::Initialize(const FnEventCallback& windowCallback)
	{
		m_Window = tur::MakeUnique<Window>();
		m_Window->Initialize({ });
		m_Window->SetEventCallback(windowCallback);
	}

	void GraphicsSystem::SetupWindow(const WindowProperties& properties)
	{
		m_Window->Initialize(properties);
	}

	void GraphicsSystem::SelectGraphicsBackend(BackendType type, const BackendProperties& properties)
	{
		m_Backend = MakeGraphicsBackend(type, properties);

		WindowProperties windowProperties = m_Window ? m_Window->GetProperties() : WindowProperties{ };
		m_Window->Initialize(windowProperties);

		m_Backend->FinishSetup(m_Window);
	}
}
