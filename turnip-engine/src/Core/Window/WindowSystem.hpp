#pragma once
#include "Graphics/GraphicsSpecification.h"
#include "Core/Event/Events.h"
#include "Platform/Platform.h"
#include "WindowProperties.h"

namespace tur
{
	class WindowSystem
	{
	public:
		void Initialize(const WindowProperties&, const GraphicsSpecification& graphicsSpecification = {});

		void SetEventCallback(const EventCallback& callback);

		void Shutdown();

	private:
		void InitializeOpenGL(const WindowProperties&, const GraphicsSpecification&);

		void InitializeVulkan(const WindowProperties&, const GraphicsSpecification&);

	public:
		const platform::Window& GetWindow() const { return m_Window; }

		platform::Window& GetWindow() { return m_Window; }

	private:
		static inline bool isWindowingInitialized = false;
		platform::Window m_Window;
	};
}