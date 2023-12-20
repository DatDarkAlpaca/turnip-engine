#pragma once
#include "Common.h"
#include "TurnipEngineData.h"

#include "Core/Event/Event.h"
#include "Core/View/ViewHolder.h"

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
		tur_shared<IGraphicsBackend> CreateGraphicsAPI(BackendType type, const BackendProperties& properties);

		void CreateWindow(const WindowProperties& properties);

	public:
		void AddView(tur::tur_unique<View> view);

	public:
		void Run();

	private:
		void OnEngineInitialize();

		void OnRender();

		void OnRenderGUI();

		void OnUpdate();

		void OnEvent(Event& event);

	public:
		const TurnipEngineData& Data() const { return m_Data; }
		TurnipEngineData& Data() { return m_Data; }
		
	private:
		LoggerSystem* g_LoggerSystem = nullptr;

		TurnipEngineData m_Data;
	};


}