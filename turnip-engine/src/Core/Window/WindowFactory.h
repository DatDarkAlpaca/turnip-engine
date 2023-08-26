#pragma once
#include "Platform/Platform.h"

namespace tur
{
	class WindowFactory
	{
		using EventCallback = std::function<void(Event&)>;

	public:
		WindowFactory(const WindowProperties& properties)
			: m_Properties(properties) 
		{
			if (!m_WindowLoader.Preload())
			{
				TUR_CORE_CRITICAL("Failed to preload window.");
				return;
			}
		}

	public:
		std::unique_ptr<Window> Create(const EventCallback& eventCallback)
		{
			m_Window = std::make_unique<Window>(m_Properties);
			m_Window->SetEventCallback(eventCallback);
			
			return std::move(m_Window);
		}

	public:
		Window& GetWindow() const { return *m_Window.get(); }

	private:
		std::unique_ptr<Window> m_Window;
		WindowProperties m_Properties;
		WindowLoader m_WindowLoader;
	};
}