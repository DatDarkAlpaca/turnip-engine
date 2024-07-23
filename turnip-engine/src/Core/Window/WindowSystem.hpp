#pragma once
#include "Core/Event/Events.hpp"
#include "Platform/Platform.hpp"

// TODO: consider un-wrapping this system.

namespace tur
{
	class WindowSystem
	{
	public:
		void SetEventCallback(const EventCallback& callback);

		void Shutdown();

	public:
		const platform::Window& GetWindow() const { return m_Window; }

		platform::Window& GetWindow() { return m_Window; }

	private:
		static inline bool isWindowingInitialized = false;
		platform::Window m_Window;
	};
}