#pragma once
#include <GLFW/glfw3.h>

#include "Common.h"
#include "Core/Event/Event.h"
#include "Core/Window/WindowBase.h"

namespace tur
{
	class Event;

	class GLFWWindowDestroyer
	{
	public:
		void operator() (GLFWwindow* window);
	};
}

namespace tur
{
	class WindowGLFW
	{
	public:
		void Initialize(const WindowProperties& properties);

	public:
		void SetEventCallback(const FnEventCallback& callback);

		void SetProperties(const WindowProperties& properties);

		void PollEvents();

	public:
		void Hide();

		void Show();

	public:
		WindowProperties GetProperties() const { return m_Properties; }

		bool IsOpen() const;

	private:
		void SetWindowDataPointer();

		void SetWindowCallbacks();

	private:
		tur_unique<GLFWwindow, GLFWWindowDestroyer> m_Window;
		WindowProperties m_Properties;

		struct WindowData
		{
			FnEventCallback eventCallback;
		} m_WindowData;
	};
}