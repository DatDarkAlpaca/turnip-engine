#pragma once
#include "Core/Config/ConfigSystem.hpp"
#include "Core/Event/Events.hpp"
#include "Graphics/UI/UIBackend.hpp"
#include "Platform/Platform.hpp"

namespace tur
{
	class WindowSystem
	{
	public:
		void Initialize(const ConfigSystem& configSystem);

		void SetEventCallback(const EventCallback& callback);

		void Shutdown();

	private:
		void InitializeOpenGL(const ConfigSystem& configSystem);

		void InitializeVulkan(const ConfigSystem& configSystem);

	public:
		const platform::Window& GetWindow() const { return m_Window; }

		platform::Window& GetWindow() { return m_Window; }

	private:
		static inline bool isWindowingInitialized = false;
		platform::Window m_Window;
	};
}