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
		glfwSetErrorCallback(GLFW_ErrorCallback);

		// Mouse Callbacks:
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow*, double x, double y) {
			Mouse::SetPosition({ x, y });
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow*, int button, int action, int mods) {
			Mouse::SetButtonState(button, action, mods);
		});

		// Keyboard Callbacks:
		glfwSetKeyCallback(m_Window, [](GLFWwindow*, int key, int scancode, int action, int mods) {
			Keyboard::SetKeyState(key, scancode, action, mods);
		});
	}

	void Window::Destroy()
	{
		glfwDestroyWindow(m_Window);
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