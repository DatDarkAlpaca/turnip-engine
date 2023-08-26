#pragma once
#include "pch.h"
#ifdef TUR_WINDOWING_GLFW
#include "Core/Window/WindowProperties.h"

namespace tur
{
	class Event;

	struct GLFWWindowDeleter
	{
		void operator()(GLFWwindow* window)
		{
			glfwDestroyWindow(window);
		}
	};

	class Window
	{
		using FnEventCallback = std::function<void(Event&)>;

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
		void SetupCallbacks() const;

	public:
		WindowProperties GetProperties() const { return m_Properties; }

	private:
		std::unique_ptr<GLFWwindow, GLFWWindowDeleter> m_Window;
		WindowProperties m_Properties;

		struct WindowData
		{
			FnEventCallback eventCallback;
		} m_WindowData;
	};
}
#endif