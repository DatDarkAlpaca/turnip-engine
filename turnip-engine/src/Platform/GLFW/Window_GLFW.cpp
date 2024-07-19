#include "pch.hpp"
#include "Common.hpp"

#include "Core/Event/Events.hpp"
#include "Events_GLFW.hpp"

#include "Window_GLFW.hpp"
#include "Monitor_GLFW.hpp"

namespace tur::platform
{
	void GLFWWindowDestroyer::operator()(GLFWwindow* window)
	{
		glfwDestroyWindow(window);
	}
}

namespace tur::platform
{
	void Window::Initialize(const WindowProperties& properties)
	{
		m_Properties = properties;

		auto* glfwWindow = glfwCreateWindow(
			(int)properties.dimensions.x,
			(int)properties.dimensions.y,
			properties.windowTitle.c_str(),
			nullptr, nullptr
		);

		if (!glfwWindow)
			TUR_LOG_CRITICAL("Failed to create a valid GLFW window. GLFW is probably uninitialized");

		m_Window.reset(glfwWindow);

		// Window Data:
		SetWindowDataPointer();
		SetWindowCallbacks();
	}

	void Window::Shutdown()
	{
		m_Window.reset();
	}

	void Window::SetEventCallback(const EventCallback& callback)
	{
		m_WindowData.eventCallback = callback;
	}

	inline void Window::SetProperties(const WindowProperties& properties)
	{
		auto* window = m_Window.get();
		m_Properties = properties;

		// Current Size:
		glfwSetWindowSize(window, (int)properties.dimensions.x, (int)properties.dimensions.y);
		
		// Size Limits:
		glfwSetWindowSizeLimits(
			window,
			(int)properties.minSize.x,
			(int)properties.minSize.y,
			(int)properties.maxSize.x,
			(int)properties.maxSize.y
		);

		// Position:
		int x = (int)properties.position.x, y = (int)properties.position.y;
		if (properties.position.x == WindowProperties::Position::DEFAULT)
			x = GLFW_DONT_CARE;
		if (properties.position.y == WindowProperties::Position::DEFAULT)
			y = GLFW_DONT_CARE;
			
		glfwSetWindowPos(window, x, y);

		// Title:
		glfwSetWindowTitle(window, properties.windowTitle.c_str());
	}

	void Window::PollEvents()
	{
		glfwPollEvents();
	}

	void Window::Hide()
	{
		glfwHideWindow(m_Window.get());
	}

	void Window::Show()
	{
		glfwShowWindow(m_Window.get());
	}

	bool Window::IsOpen() const
	{
		return !glfwWindowShouldClose(m_Window.get());
	}
	
	void Window::SetWindowDataPointer()
	{
		glfwSetWindowUserPointer(m_Window.get(), &m_WindowData);
	}

	void Window::SetWindowCallbacks()
	{
		// Status:
		glfwSetWindowCloseCallback(m_Window.get(), [](GLFWwindow* window) {
			auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			WindowCloseEvent event;
			data->eventCallback(event);
		});

		glfwSetWindowMaximizeCallback(m_Window.get(), [](GLFWwindow* window, int maximized) {
			auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			WindowMaximizedEvent event(maximized);
			data->eventCallback(event);
		});

		glfwSetWindowFocusCallback(m_Window.get(), [](GLFWwindow* window, int focus) {
			auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			WindowFocusEvent event(focus);
			data->eventCallback(event);
		});

		// Size:
		glfwSetWindowSizeCallback(m_Window.get(), [](GLFWwindow* window, int width, int height) {
			auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			WindowResizeEvent event(width, height);
			data->eventCallback(event);
		});

		glfwSetFramebufferSizeCallback(m_Window.get(), [](GLFWwindow* window, int width, int height) {
			auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			WindowFramebufferEvent event(width, height);
			data->eventCallback(event);
		});


		glfwSetWindowContentScaleCallback(m_Window.get(), [](GLFWwindow* window, float xScale, float yScale) {
			auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			WindowContentScaleEvent event(xScale, yScale);
			data->eventCallback(event);
		});

		// Position:
		glfwSetWindowPosCallback(m_Window.get(), [](GLFWwindow* window, int xPos, int yPos) {
			auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			WindowMovedEvent event(xPos, yPos);
			data->eventCallback(event);
		});

		// Keyboard:
		glfwSetKeyCallback(m_Window.get(), [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			
			switch (action)
			{
				case GLFW_PRESS:
				{
					Key convertedKey = TranslateKey(key);
					KeyboardMods convertedMods = TranslateKeyMods(mods);

					KeyPressedEvent event(convertedKey, convertedMods);
					data->eventCallback(event);
				} break;
				
				case GLFW_RELEASE:
				{
					Key convertedKey = TranslateKey(key);
					KeyboardMods convertedMods = TranslateKeyMods(mods);

					KeyReleasedEvent event(convertedKey, convertedMods);
					data->eventCallback(event);
				} break;

				case GLFW_REPEAT:
				{
					Key convertedKey = TranslateKey(key);
					KeyboardMods convertedMods = TranslateKeyMods(mods);

					KeyRepeatEvent event(convertedKey, convertedMods);
					data->eventCallback(event);	
				} break;

				default:
				{
					TUR_LOG_WARN("Invalid keyboard action passed to window callback: {}", action);
				} break;
			}
		});
	
		// Mouse:
		glfwSetCursorPosCallback(m_Window.get(), [](GLFWwindow* window, double x, double y) {
			auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			MouseMovedEvent event(x, y);
			data->eventCallback(event);
		});

		glfwSetMouseButtonCallback(m_Window.get(), [](GLFWwindow* window, int button, int action, int mods) {
			auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButton convertedButton = TranslateMouseButton(button);
					MousePressedEvent event(convertedButton);
					data->eventCallback(event);
				} break;

				case GLFW_RELEASE:
				{
					MouseButton convertedButton = TranslateMouseButton(button);
					MouseReleasedEvent event(convertedButton);
					data->eventCallback(event);
				} break;

				default:
				{
					TUR_LOG_WARN("Invalid keyboard action passed to window callback: {}", action);
				} break;
			}
		});

		glfwSetScrollCallback(m_Window.get(), [](GLFWwindow* window, double xOffset, double yOffset) {
			auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			MouseScrollEvent event(xOffset, yOffset);
			data->eventCallback(event);
		});
	}
}