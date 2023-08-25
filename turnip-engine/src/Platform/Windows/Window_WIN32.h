#pragma once
#include "pch.h"
#ifdef TUR_WINDOWING_WINDOWS
#include "System/Window/BaseWindow.h"

namespace tur
{
	class Window_WIN32 : public BaseWindow<Window_WIN32>
	{
	public:
		Window_WIN32(const WindowProperties& properties);

		~Window_WIN32();

	public:
		void SetEventCallback(const FnEventCallback& eventCallback);

		void PollEvents();

	public:
		void Show();

		void Hide();

	public:
		bool IsOpen() const;

	public:
		void* Get() const;

	private:
		static LRESULT CALLBACK SetupMessagePump(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

		static LRESULT CALLBACK PumpMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

		LRESULT HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	public:
		HWND GetHandle() const { return m_Handle; }

	private:
		static void InitializeClass();

		void InitializeWindow();

	private:
		FnEventCallback m_EventCallback;
		HWND m_Handle = nullptr;
		bool m_Open = false;

	private:
		static constexpr const wchar_t* ClassName = L"TurnipEngineWindow";
	};
}
#endif