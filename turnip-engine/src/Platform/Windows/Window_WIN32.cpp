#include "pch.h"
#ifdef TUR_WINDOWING_WINDOWS
#include "Window_WIN32.h"

namespace tur
{
	Window_WIN32::Window_WIN32(const WindowProperties& properties)
		: BaseWindow(properties)
	{
		SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);

		Window_WIN32::InitializeClass();

		InitializeWindow();

		if (m_Handle)
			m_Open = true;

		Show();
	}

	Window_WIN32::~Window_WIN32()
	{
		DestroyWindow(m_Handle);
	}

	void Window_WIN32::SetEventCallback(const FnEventCallback& eventCallback)
	{
		m_EventCallback = eventCallback;
	}

	void Window_WIN32::PollEvents()
	{
		MSG msg;
		while (PeekMessageW(&msg, m_Handle, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	void Window_WIN32::Show()
	{
		TUR_ASSERT(m_Handle, "Attempted to call Show() on an uninitialized window");
		ShowWindow(m_Handle, SW_SHOWNORMAL);
	}

	void Window_WIN32::Hide()
	{
		TUR_ASSERT(m_Handle, "Attempted to call Hide() on an uninitialized window");
		ShowWindow(m_Handle, SW_HIDE);
	}

	bool Window_WIN32::IsOpen() const
	{
		return m_Open;
	}

	void* Window_WIN32::Get() const
	{
		return m_Handle;
	}

	LRESULT Window_WIN32::SetupMessagePump(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		if (msg != WM_NCCREATE)
			return DefWindowProc(hWnd, msg, wParam, lParam);

		const CREATESTRUCTW* const createStruct = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window_WIN32* const window = static_cast<Window_WIN32*>(createStruct->lpCreateParams);

		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window_WIN32::PumpMessage));

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT Window_WIN32::PumpMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		Window_WIN32* const window = reinterpret_cast<Window_WIN32*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		return window->HandleMessage(hWnd, msg, wParam, lParam);
	}

	// TODO: Event queue
	LRESULT Window_WIN32::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		switch (msg)
		{
			case WM_CLOSE:
			{
				m_Open = false;
			} break;
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	void Window_WIN32::InitializeClass()
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

	void Window_WIN32::InitializeWindow()
	{
		DWORD dwStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

		dwStyle |= WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
		DWORD exStyle = WS_EX_APPWINDOW | WS_EX_TOPMOST;

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
#endif