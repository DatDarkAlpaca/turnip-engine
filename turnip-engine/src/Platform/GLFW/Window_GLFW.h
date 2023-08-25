#pragma once
#include "System/Window/BaseWindow.h"

namespace tur
{
	struct GLFWWindowDeleter
	{
		void operator()(GLFWwindow* window)
		{
			glfwDestroyWindow(window);
		}
	};

	class Window : public BaseWindow<Window>
	{
	public:
		Window(const WindowProperties& properties);

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
		std::unique_ptr<GLFWwindow, GLFWWindowDeleter> m_Window;

		struct WindowData
		{
			FnEventCallback eventCallback;
		} m_WindowData;

	private:
		static inline bool s_IsGLFWLoaded = false;
	};
}