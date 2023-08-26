#pragma once
#include "TurnipEngineState.h"
#include "View/ViewQueue.h"
#include "Core/Event/Events.h"
#include "Graphics/GraphicsSystem.h"

#include "Platform/Platform.h"

namespace tur
{
	class TurnipEngine
	{
	public:
		TurnipEngine();

		~TurnipEngine();

	private:
		void Setup();

	public:
		virtual void Initialize() { }

		virtual void Shutdown() { }

	public:
		void Run();

	private:
		void OnEvent(Event& event);

		void OnUpdate();

		void OnRenderGUI();

	public:
		void SwapGraphicsAPI(GraphicsAPI api);

	public:
		static inline TurnipEngine& Get() { return *s_Instance; }

	protected:
		std::unique_ptr<Window> window;
		GraphicsSystem graphics;
		ViewQueue viewQueue;

	private:
		static inline TurnipEngine* s_Instance = nullptr;
		TurnipEngineState m_State;
	};
}

#define CREATE_APPLICATION(applicationClass, ...)		\
tur::TurnipEngine* CreateApp()							\
{														\
	return new applicationClass(__VA_ARGS__);			\
}