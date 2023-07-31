#include "pch.h"
#ifdef TUR_PLATFORM_WINDOWS

#include "Core/Event/Events.h"
#include "WIN32_Window.h"

namespace tur
{
	WIN32_Window::WIN32_Window(const WindowProperties& properties)
		: IWindow(properties)
	{
		// TODO: Handle win7 dpi awareness.
		SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
		
		WIN32_Window::InitializeClass();

		InitializeWindow();

		if (m_Handle)
			m_Open = true;
	}

	WIN32_Window::~WIN32_Window()
	{
		DestroyWindow(m_Handle);
	}

	void WIN32_Window::SetEventCallback(const FnEventCallback& eventCallback)
	{
		m_EventCallback = eventCallback;
	}

	void WIN32_Window::PollEvents()
	{
		MSG msg;
		while (PeekMessageW(&msg, m_Handle, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	void WIN32_Window::Show()
	{
		TUR_ASSERT(m_Handle, "Attempted to call Show() on an uninitialized window");
		ShowWindow(m_Handle, SW_SHOWNORMAL);
	}

	void WIN32_Window::Hide()
	{
		TUR_ASSERT(m_Handle, "Attempted to call Hide() on an uninitialized window");
		ShowWindow(m_Handle, SW_HIDE);
	}

	bool WIN32_Window::IsOpen() const
	{
		return m_Open;
	}

	void* WIN32_Window::GetWindow() const
	{
		return m_Handle;
	}

	LRESULT WIN32_Window::SetupMessagePump(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		if (msg != WM_NCCREATE)
			return DefWindowProc(hWnd, msg, wParam, lParam);

		const CREATESTRUCTW* const createStruct = reinterpret_cast<CREATESTRUCTW*>(lParam);
		WIN32_Window* const window = static_cast<WIN32_Window*>(createStruct->lpCreateParams);

		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WIN32_Window::PumpMessage));

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT WIN32_Window::PumpMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		WIN32_Window* const window = reinterpret_cast<WIN32_Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		return window->HandleMessage(hWnd, msg, wParam, lParam);
	}

	LRESULT WIN32_Window::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		switch (msg)
		{
			case WM_CLOSE:
			{
				m_Open = false;
			} break;

			case WM_SIZE:
			{ 
				if (!m_EventCallback)
					break;

				WindowResizeEvent event((int)LOWORD(lParam), (int)HIWORD(lParam));
				m_EventCallback(event);
			} break;
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	void WIN32_Window::InitializeClass()
	{
		WNDCLASSEX wndClass = {};
		wndClass.cbSize = sizeof(wndClass);
		wndClass.style = CS_OWNDC;
		wndClass.lpfnWndProc = SetupMessagePump;
		wndClass.hInstance = GetModuleHandleA(nullptr);
		wndClass.lpszClassName = ClassName;

		if (FAILED(RegisterClassEx(&wndClass)))
		{
			TUR_CORE_CRITICAL("Failed to register WIN32 class. | Error: {}", GetLastError());
			return;
		}
	}

	void WIN32_Window::InitializeWindow()
	{
		DWORD dwStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

		dwStyle |= WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
		DWORD exStyle = WS_EX_APPWINDOW | WS_EX_TOPMOST;
		// TODO: window properties.

		RECT rect = {};
		rect.right = (int)properties.dimensions.x;
		rect.bottom = (int)properties.dimensions.y;

		if (!AdjustWindowRectEx(&rect, dwStyle, true, exStyle))
			TUR_CORE_WARN("Failed to adjust the WIN32 window rect. | Error: {}", GetLastError());

		std::wstring title = std::wstring(properties.title.begin(), properties.title.end());

		m_Handle = CreateWindowEx(
			exStyle,
			ClassName,
			title.c_str(),
			dwStyle,
			(int)properties.position.x,
			(int)properties.position.y,
			rect.right - rect.left,
			rect.bottom - rect.top,
			nullptr, nullptr, GetModuleHandleA(nullptr), this
		);

		if (!m_Handle)
		{
			TUR_CORE_CRITICAL("Failed to create WIN32 window. | Error Code: {}", GetLastError());
			return;
		}

		// Adjust the window dimensions:
		RECT windowRect, clientRect;
		GetWindowRect(m_Handle, &windowRect);
		GetClientRect(m_Handle, &clientRect);

		int xExtra = windowRect.right - windowRect.left - clientRect.right;
		int yExtra = windowRect.bottom - windowRect.top - clientRect.bottom;

		SetWindowPos(
			m_Handle, nullptr, 
			(int)properties.position.x,
			(int)properties.position.y,
			(int)properties.dimensions.x + xExtra,
			(int)properties.dimensions.y + yExtra,
			SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE
		);
	}
}
#endif // TUR_PLATFORM_WINDOWS