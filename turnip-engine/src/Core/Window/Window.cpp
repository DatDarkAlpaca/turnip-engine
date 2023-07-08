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

			KeyboardEvent event(key, scancode, action, mods);
			data->eventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
			auto* data = static_cast<UserPointerData*>(glfwGetWindowUserPointer(window));

			MousePositionEvent event(xPos, yPos);
			data->eventCallback(event);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
			auto* data = static_cast<UserPointerData*>(glfwGetWindowUserPointer(window));

			MouseButtonEvent event(button, action, mods);
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

	void Window::Resize(const glm::vec2& size)
	{
		glfwSetWindowSize(m_Window, size.x, size.y);
		SetViewport(size);
	}

	void Window::Rename(const char* title)
	{
		glfwSetWindowTitle(m_Window, title);
	}
}