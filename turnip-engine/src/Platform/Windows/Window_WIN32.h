#pragma once
#ifdef TUR_WINDOWING_WINDOWS
#include <functional>
#include "Platform/Windows/WindowsHeaders.h"
#include "Core/Window/WindowProperties.h"

namespace tur
{
	class Event;

	class Window
	{
		using FnEventCallback = std::function<void(Event&)>;

	public:
		Window(const WindowProperties& properties);

		~Window();

	private:
		static void InitializeClass();

		void InitializeWindow();

	public:
		void SetEventCallback(const FnEventCallback& eventCallback);

		void PollEvents();

	public:
		void Show();

		void Hide();

	public:
		bool IsOpen() const;

	public:
		HWND Get() const;

	private:
		static LRESULT CALLBACK SetupMessagePump(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

		static LRESULT CALLBACK PumpMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

		LRESULT HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	public:
		WindowProperties GetProperties() const { return m_Properties; }

	private:
		FnEventCallback m_EventCallback;
		WindowProperties m_Properties;
		HWND m_Handle = nullptr;
		bool m_Open = false;

	private:
		static constexpr const wchar_t* ClassName = L"TurnipEngineWindow";
	};
}
#endif