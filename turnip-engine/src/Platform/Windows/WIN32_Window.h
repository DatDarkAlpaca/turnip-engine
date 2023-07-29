#pragma once
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

	private:
		static void InitializeClass();

		void InitializeWindow();

	private:
		HWND m_Handle = nullptr;
		bool m_Open = false;

	private:
		static constexpr const wchar_t* ClassName = L"TurnipEngineWindow";
	};
}