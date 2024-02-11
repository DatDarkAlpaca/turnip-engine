#pragma once
#include "Common.h"

#include "Core/View/ViewSystem.h"
#include "Core/Event/Event.h"
#include "Platform/Platform.h"

namespace tur
{
	class TurnipEngine
	{
		friend class GraphicsEngineWrapper;

	public:
		TurnipEngine();

	public:
		void Run();

	private:
		void Initialize();

		void Shutdown();

	private:
		void OnEngineStartup();

		void OnRender();

		void OnRenderGUI();

		void OnUpdate();

		void OnEvent(Event& event);

		void OnEngineShutdown();
		
	public:
		ViewSystem& View() { return g_ViewSystem.Get(); }

	private:
		LoggerSystem g_LoggerSystem;
		ViewSystem g_ViewSystem;
		Window g_Window;

	private:
		bool m_Initialized = false;
	};
}