#pragma once
#include "TurnipEngineState.h"
#include "View/ViewQueue.h"
#include "Event/Events.h"

#include "Window/Window.h"
#include "Graphics/API/GraphicsAPI.h"

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
		void OnEvent(IEvent& event);

		void OnUpdate();

		void OnRenderGUI();

	public:
		void SwapGraphicsSystem(GraphicsAPI_Type graphicsAPI, const WindowProperties& properties = {});

	public:
		static inline TurnipEngine& Get() { return *s_Instance; }

	protected:
		std::unique_ptr<Window> window;
		std::unique_ptr<IGraphicsAPI> api; // Todo: replace with renderer

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