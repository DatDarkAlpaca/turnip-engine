#include "pch.h"
#ifdef TUR_WINDOWING_GLFW
#include "GLFW_Window.h"
#include "Core/Event/Events.h"

namespace tur
{
	GLFW_Window::GLFW_Window(const WindowProperties& properties)
		: IWindow(properties)
	{
		int width = (int)properties.dimensions.x;
		int height = (int)properties.dimensions.y;
		int x = (int)properties.position.x;
		int y = (int)properties.position.y;

		TUR_ASSERT(width > 0,  "Window dimensions must be integers greater than zero");
		TUR_ASSERT(height > 0, "Window dimensions must be integers greater than zero");
		TUR_ASSERT(x > 0,      "Window coordinates must be integers greater than zero");
		TUR_ASSERT(y > 0,      "Window coordinates must be integers greater than zero");

		GLFWwindow* window = glfwCreateWindow(width, height, properties.title.c_str(), nullptr, nullptr);
		glfwSetWindowPos(window, x, y);

		if (!window)
		{
			TUR_CORE_CRITICAL("Failed to create a GLFW window");
			return;
		}

		m_Window.reset(window);
		SetupCallbacks();
	}

	void GLFW_Window::SetEventCallback(const FnEventCallback& eventCallback)
	{
		m_WindowData.eventCallback = eventCallback;
	}

	void GLFW_Window::PollEvents()
	{
		glfwPollEvents();
	}

	void GLFW_Window::Show()
	{
		TUR_ASSERT(m_Window, "Attempted to call Show() on an uninitialized window");
		glfwShowWindow(m_Window.get());
	}

	void GLFW_Window::Hide()
	{
		TUR_ASSERT(m_Window, "Attempted to call Hide() on an uninitialized window");
		glfwHideWindow(m_Window.get());
	}

	bool GLFW_Window::IsOpen() const
	{
		return !glfwWindowShouldClose(m_Window.get());
	}

	void* GLFW_Window::GetWindow() const
	{
		return m_Window.get();
	}

	void GLFW_Window::SetupCallbacks()
	{
		glfwSetWindowUserPointer(m_Window.get(), &m_WindowData);

		glfwSetWindowSizeCallback(m_Window.get(), [](GLFWwindow* window, int width, int height) {
			auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			WindowResizeEvent event(width, height);
			data->eventCallback(event);
		});
	}
}
#endif // TUR_WINDOWING_GLFW