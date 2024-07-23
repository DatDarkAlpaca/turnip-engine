#include "pch.hpp"
#include "WindowSystem.hpp"

namespace tur
{
	void WindowSystem::SetEventCallback(const EventCallback& callback)
	{
		m_Window.SetEventCallback(callback);
	}

	void WindowSystem::Shutdown()
	{
		m_Window.Shutdown();
	}
}
