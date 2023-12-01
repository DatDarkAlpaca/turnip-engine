#pragma once
#include "Common.h"
#include "TurnipEngineData.h"

#include "Core/Event/Event.h"
#include "Core/View/ViewHolder.h"

#include "Graphics/BackendType.h"

namespace tur
{
	class TurnipEngine
	{
	public:
		TurnipEngine();

	public:
		void SwitchGraphicsAPI(BackendType type, const BackendProperties& properties);

		void CreateWindow(const WindowProperties& properties);

	public:
		void AddView(tur::tur_unique<View> view);

	public:
		void Run();

	private:
		void OnRender();

		void OnRenderGUI();

		void OnUpdate();

		void OnEvent(Event& event);

	public:
		const TurnipEngineData& Data() const { return m_Data; }
		TurnipEngineData& Data() { return m_Data; }

	private:
		TurnipEngineData m_Data;
	};
}