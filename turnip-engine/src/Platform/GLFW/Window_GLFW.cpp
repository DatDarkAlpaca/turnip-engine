#include "pch.h"
#ifdef TUR_WINDOWING_GLFW
#include "Window_GLFW.h"
#include "Core/Event/Events.h"

namespace tur
{
	Window::Window(const WindowProperties& properties)
		: m_Properties(properties)
	{
		int width = (int)properties.dimensions.x;
		int height = (int)properties.dimensions.y;
		int x = (int)properties.position.x;
		int y = (int)properties.position.y;

		TUR_ASSERT(width > 0, "Window dimensions must be integers greater than zero");
		TUR_ASSERT(height > 0, "Window dimensions must be integers greater than zero");
		TUR_ASSERT(x > 0, "Window coordinates must be integers greater than zero");
		TUR_ASSERT(y > 0, "Window coordinates must be integers greater than zero");

		GLFWwindow* window = glfwCreateWindow(width, height, properties.title.c_str(), nullptr, nullptr);
		if (!window)
		{
			TUR_CORE_CRITICAL("Failed to create a GLFW window");
			return;
		}

		glfwSetWindowPos(window, x, y);

		m_Window.reset(window);
		glfwSetWindowUserPointer(m_Window.get(), &m_WindowData);

		SetupCallbacks();
	}

	void Window::SetEventCallback(const FnEventCallback& eventCallback)
	{
		m_WindowData.eventCallback = eventCallback;
	}

	void Window::PollEvents()
	{
		glfwPollEvents();
	}

	void Window::Show()
	{
		TUR_ASSERT(m_Window, "Attempted to call Show() on an uninitialized window");
		glfwShowWindow(m_Window.get());
	}

	void Window::Hide()
	{
		TUR_ASSERT(m_Window, "Attempted to call Hide() on an uninitialized window");
		glfwHideWindow(m_Window.get());
	}

	bool Window::IsOpen() const
	{
		return !glfwWindowShouldClose(m_Window.get());
	}

	GLFWwindow* Window::Get() const
	{
		return m_Window.get();
	}

	void Window::SetupCallbacks() const
	{
		glfwSetWindowCloseCallback(m_Window.get(), [](GLFWwindow* window) {
			auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			Event event(EventType::WINDOW_CLOSE, std::make_unique<WindowCloseEventData>());
			data->eventCallback(event);
		});

		glfwSetWindowSizeCallback(m_Window.get(), [](GLFWwindow* window, int width, int height) {
			auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			Event event(EventType::WINDOW_RESIZE, std::make_unique<WindowResizeEventData>((unsigned int)width, (unsigned int)height));
			data->eventCallback(event);
		});
	}
}
#endif