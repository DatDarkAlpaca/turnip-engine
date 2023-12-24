#pragma once
#include "Common.h"
#include "Core/Event/Event.h"
#include "Core/Window/WindowBase.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

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

		void Shutdown();

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

		void* GetHandle() const
		{
			return m_Window.get();
		}

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

	using Window = WindowBase<WindowGLFW>;
}