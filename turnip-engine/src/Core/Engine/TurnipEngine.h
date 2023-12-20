#pragma once
#include "Common.h"
#include "Core/Event/Event.h"
#include "Core/View/ViewSystem.h"

#include "Graphics/Graphics.h"

#define BACKEND(BackendClass) static_cast<BackendClass*>(Graphics());

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
		void OnEngineInitialize();

		void OnRender();

		void OnRenderGUI();

		void OnUpdate();

		void OnEvent(Event& event);
		
	public:
		ViewSystem& View() const { return g_ViewSystem->Get(); }
		GraphicsSystem& Graphics() const { return g_GraphicsSystem->Get(); }

	private:
		LoggerSystem* g_LoggerSystem = nullptr;
		ViewSystem* g_ViewSystem = nullptr;
		GraphicsSystem* g_GraphicsSystem = nullptr;

		bool m_Initialized = false;
	};
}