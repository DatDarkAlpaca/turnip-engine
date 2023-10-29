#include "pch.h"
#include "Common.h"
#include "Window_GLFW.h"
#include "Monitor_GLFW.h"
#include "Core/Event/Events.h"
#include "Core/Event/EventType.h"

namespace tur
{
	tur::tur_unique<Window> Window::Create(const WindowProperties& properties)
	{
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

		int width = (int)properties.dimensions.x;
		int height = (int)properties.dimensions.y;
		int x = (int)properties.position.x;
		int y = (int)properties.position.y;

		TUR_ASSERT(width > 0, "Window dimensions must be integers greater than zero");
		TUR_ASSERT(height > 0, "Window dimensions must be integers greater than zero");

		return tur::MakeUnique<WindowGLFW>(properties);
	}
}

namespace tur
{
	void GLFWWindowDestroyer::operator()(GLFWwindow* window)
	{
		glfwDestroyWindow(window);
	}

	WindowGLFW::WindowGLFW(const WindowProperties& properties)
	{
		auto* glfwWindow = glfwCreateWindow(
			(int)m_Properties.dimensions.x,
			(int)m_Properties.dimensions.y,
			m_Properties.windowTitle.c_str(),
			nullptr, nullptr
		);

		if (!glfwWindow)
			TUR_LOG_CRITICAL("Failed to create a valid GLFW window. GLFW is probably uninitialized");

		m_Window.reset(glfwWindow);

		// Default size limits:
		glm::vec2 maximumSize =
			m_Properties.maxSize.x > 0 && m_Properties.maxSize.y > 0
			? m_Properties.maxSize : glm::vec2(GLFW_DONT_CARE, GLFW_DONT_CARE);

		SetSizeLimits(m_Properties.minSize, maximumSize);
		SetPosition(m_Properties.position);

		// Window Data:
		SetWindowDataPointer();
		SetWindowCallbacks();
	}

	void WindowGLFW::PollEvents()
	{
		glfwPollEvents();
	}

	void WindowGLFW::SetEventCallback(const FnEventCallback& callback)
	{
		m_WindowData.eventCallback = callback;
	}

	void WindowGLFW::Hide()
	{
		glfwHideWindow(m_Window.get());
	}

	void WindowGLFW::Show()
	{
		glfwShowWindow(m_Window.get());
	}

	void WindowGLFW::SetPosition(const glm::vec2& position)
	{
		glm::vec2 oldPosition = GetPosition();

		int x = (int)position.x == (int)WindowPosition::DEFAULT ? (int)oldPosition.x : (int)position.x;
		int y = (int)position.y == (int)WindowPosition::DEFAULT ? (int)oldPosition.y : (int)position.y;

		glfwSetWindowPos(m_Window.get(), x, y);
	}

	void WindowGLFW::SetSizeLimits(const glm::vec2& minimumSize, const glm::vec2& maximumSize)
	{
		glfwSetWindowSizeLimits(
			m_Window.get(), 
			(int)minimumSize.x,
			(int)minimumSize.y,
			(int)maximumSize.x, 
			(int)maximumSize.y
		);

		m_Properties.minSize = minimumSize;
		m_Properties.maxSize = maximumSize;
	}

	glm::vec2 WindowGLFW::GetPosition() const
	{
		int oldX, oldY;
		glfwGetWindowPos(m_Window.get(), &oldX, &oldY);

		return glm::vec2(oldX, oldY);
	}

	bool WindowGLFW::IsOpen() const
	{
		return !glfwWindowShouldClose(m_Window.get());
	}
	
	void WindowGLFW::SetWindowDataPointer()
	{
		glfwSetWindowUserPointer(m_Window.get(), &m_WindowData);
	}

	void WindowGLFW::SetWindowCallbacks()
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
					KeyPressedEvent event((Key)key, KeyboardMods(mods));
					data->eventCallback(event);
				} break;
				
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event((Key)key, KeyboardMods(mods));
					data->eventCallback(event);
				} break;

				case GLFW_REPEAT:
				{
					KeyRepeatEvent event((Key)key, KeyboardMods(mods));
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
					MousePressedEvent event((MouseButton)button);
					data->eventCallback(event);
				} break;

				case GLFW_RELEASE:
				{
					MouseReleasedEvent event((MouseButton)button);
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
