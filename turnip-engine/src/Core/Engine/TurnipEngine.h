#pragma once
#include "Window/Window.h"
#include "View/ViewQueue.h"
#include "TurnipEngineState.h"
#include "ImGui/ImGuiView.h"

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

	public:
		Window& GetWindow() { return window; }

		static inline TurnipEngine& Get() { return *s_Instance; }

	protected:
		ImGuiView* imguiView = nullptr;
		ViewQueue viewQueue;
		Window window;

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