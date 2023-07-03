#pragma once
#include "Logger/Logger.h"
#include "Window/Window.h"
#include "Application/TurnipApplication.h"

namespace tur
{
	class TurnipEngine
	{
	public:
		TurnipEngine();

		~TurnipEngine();

	public:
		void SetApplication(TurnipApplication* application = nullptr);

	public:
		void Run();

	private:
		void InitializeSystems();

	private:
		Window m_Window;
		TurnipApplication* m_Application;

	private:
		bool m_Initialized = false;
	};
}