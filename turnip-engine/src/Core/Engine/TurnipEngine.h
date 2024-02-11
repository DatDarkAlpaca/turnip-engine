#pragma once
#include "Common.h"
#include "Core/Event/Event.h"
#include "Core/View/ViewSystem.h"

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
		void OnEngineStartup();

		void OnRender();

		void OnRenderGUI();

		void OnUpdate();

		void OnEvent(Event& event);

		void OnEngineShutdown();
		
	public:
		ViewSystem& View() { return g_ViewSystem->Get(); }

	private:
		LoggerSystem* g_LoggerSystem = nullptr;
		ViewSystem* g_ViewSystem = nullptr;

		bool m_Initialized = false;
	};
}