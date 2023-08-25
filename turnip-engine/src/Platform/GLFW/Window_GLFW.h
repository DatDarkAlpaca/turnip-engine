#pragma once
#include "pch.h"
#ifdef TUR_WINDOWING_GLFW
#include "Core/Window/BaseWindow.h"

namespace tur
{
	struct GLFWWindowDeleter
	{
		void operator()(GLFWwindow* window)
		{
			glfwDestroyWindow(window);
		}
	};

	class Window_GLFW : public BaseWindow<Window_GLFW>
	{
	public:
		Window_GLFW(const WindowProperties& properties);

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
		void SetupCallbacks() const;

	private:
		std::unique_ptr<GLFWwindow, GLFWWindowDeleter> m_Window;

		struct WindowData
		{
			FnEventCallback eventCallback;
		} m_WindowData;
	};
}
#endif