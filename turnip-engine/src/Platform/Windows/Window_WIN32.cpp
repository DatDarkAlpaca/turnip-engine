#include "pch.h"
#ifdef TUR_WINDOWING_WINDOWS
#include "Window_WIN32.h"
#include "Core/Event/Events.h"

namespace tur
{
	Window::Window(const WindowProperties& properties)
		: m_Properties(properties)
	{
		SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);

		Window::InitializeClass();

		InitializeWindow();

		if (m_Handle)
			m_Open = true;

		Show();
	}

	Window::~Window()
	{
		DestroyWindow(m_Handle);
	}

	void Window::SetEventCallback(const FnEventCallback& eventCallback)
	{
		m_EventCallback = eventCallback;
	}

	void Window::PollEvents()
	{
		MSG msg;
		while (PeekMessageW(&msg, m_Handle, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	void Window::Show()
	{
		TUR_ASSERT(m_Handle, "Attempted to call Show() on an uninitialized window");
		ShowWindow(m_Handle, SW_SHOWNORMAL);
	}

	void Window::Hide()
	{
		TUR_ASSERT(m_Handle, "Attempted to call Hide() on an uninitialized window");
		ShowWindow(m_Handle, SW_HIDE);
	}

	bool Window::IsOpen() const
	{
		return m_Open;
	}

	void* Window::Get() const
	{
		return m_Handle;
	}

	LRESULT Window::SetupMessagePump(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		if (msg != WM_NCCREATE)
			return DefWindowProc(hWnd, msg, wParam, lParam);

		const CREATESTRUCTW* const createStruct = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const window = static_cast<Window*>(createStruct->lpCreateParams);

		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::PumpMessage));

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT Window::PumpMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		Window* const window = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		return window->HandleMessage(hWnd, msg, wParam, lParam);
	}

	LRESULT Window::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		if (!m_EventCallback)
			return DefWindowProc(hWnd, msg, wParam, lParam);

		switch (msg)
		{
			case WM_DESTROY:
			case WM_CLOSE:
			{
				Event closeEvent(EventType::WINDOW_CLOSE, std::make_unique<WindowCloseEventData>());
				m_EventCallback(closeEvent);
				m_Open = false;
			} break;

			case WM_SIZE:
			{
				unsigned width, height;
				width = static_cast<unsigned>((UINT64)lParam & 0xFFFF);
				height = static_cast<unsigned>((UINT64)lParam >> 16);

				Event resizeEvent(EventType::WINDOW_RESIZE, std::make_unique<WindowResizeEventData>(width, height));
				m_EventCallback(resizeEvent);
			} break;
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	void Window::InitializeClass()
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

	void Window::InitializeWindow()
	{
		DWORD dwStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

		dwStyle |= WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
		DWORD exStyle = WS_EX_APPWINDOW;

		RECT rect = {};
		rect.right = (int)m_Properties.dimensions.x;
		rect.bottom = (int)m_Properties.dimensions.y;

		if (!AdjustWindowRectEx(&rect, dwStyle, true, exStyle))
			TUR_CORE_WARN("Failed to adjust the WIN32 window rect. | Error: {}", GetLastError());

		std::wstring title = std::wstring(m_Properties.title.begin(), m_Properties.title.end());

		m_Handle = CreateWindowEx(
			exStyle,
			ClassName,
			title.c_str(),
			dwStyle,
			(int)m_Properties.position.x,
			(int)m_Properties.position.y,
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
			(int)m_Properties.position.x,
			(int)m_Properties.position.y,
			(int)m_Properties.dimensions.x + xExtra,
			(int)m_Properties.dimensions.y + yExtra,
			SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE
		);
	}
}
#endif