#pragma once
#ifdef TUR_PLATFORM_WINDOWS

#include "pch.h"
#include "Window/IWindow.h"

namespace tur
{
	class WIN32_Window : public IWindow
	{
	public:
		WIN32_Window(const WindowProperties& properties);

		~WIN32_Window();

	public:
		void SetEventCallback(const FnEventCallback& eventCallback) override;

		void PollEvents() override;

	public:
		void Show() override;

		void Hide() override;

	public:
		bool IsOpen() const override;
		
	public:
		void* GetWindow() const override;

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

	using Window = WIN32_Window;
}
#endif // TUR_PLATFORM_WINDOWS