#include "pch.h"
#include "Window.h"

#include "Input/Mouse.h"
#include "Input/Keyboard.h"

static void GLFW_ErrorCallback(int error, const char* description)
{
	TUR_CORE_ERROR("GLFW: {} [{}]", description, error);
}

namespace tur
{
	void Window::Initialize(int width, int height, const char* title)
	{
		m_Window = glfwCreateWindow(width, height, title, nullptr, nullptr);
		if (!m_Window)
		{
			TUR_CORE_ERROR("Failed to initialize a GLFW window.");
			return;
		}

		glfwMakeContextCurrent(m_Window);

		glfwSetWindowUserPointer(m_Window, &m_UserPointerData);
		
		SetupCallbacks();
	}

	void Window::Destroy()
	{
		glfwDestroyWindow(m_Window);
	}

	void Window::SetupCallbacks()
	{
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			auto* data = static_cast<UserPointerData*>(glfwGetWindowUserPointer(window));
			
			WindowResizeEvent event(width, height);
			data->eventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			auto* data = static_cast<UserPointerData*>(glfwGetWindowUserPointer(window));

			Keyboard::SetKeyState(key, scancode, action, mods);

			KeyboardEvent event(key, scancode, action, mods);
			data->eventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
			auto* data = static_cast<UserPointerData*>(glfwGetWindowUserPointer(window));

			Mouse::SetPosition({ xPos, yPos });

			MousePositionEvent event(xPos, yPos);
			data->eventCallback(event);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
			auto* data = static_cast<UserPointerData*>(glfwGetWindowUserPointer(window));

			Mouse::SetButtonState(button, action, mods);

			MouseButtonEvent event(button, action, mods);
			data->eventCallback(event);
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double offsetX, double offsetY) {
			auto* data = static_cast<UserPointerData*>(glfwGetWindowUserPointer(window));

			Mouse::SetScrollOffset(offsetX, offsetY);

			MouseScrollEvent event(offsetX, offsetY);
			data->eventCallback(event);
		});
	}

	void Window::SetEventCallback(const FnEventCallback& callback)
	{
		m_UserPointerData.eventCallback = callback;
	}

	void Window::SetViewport(const glm::vec2& viewport)
	{
		glViewport(0, 0, (U32)viewport.x, (U32)viewport.y);
	}

	void Window::SetContextCurrent()
	{
		glfwMakeContextCurrent(m_Window);
	}

	void Window::Resize(const glm::vec2& size)
	{
		glfwSetWindowSize(m_Window, (int)size.x, (int)size.y);
		SetViewport(size);
	}

	void Window::Rename(const char* title)
	{
		glfwSetWindowTitle(m_Window, title);
	}

	glm::vec2 Window::GetSize() const
	{
		int x, y;
		glfwGetWindowSize(m_Window, &x, &y);
		return { x, y};
	}
}