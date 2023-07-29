#pragma once
#include "pch.h"
#include "Window/IWindow.h"
#include "Window/WindowProperties.h"

namespace tur
{
	struct GLFWWindowDeleter
	{
		void operator() (GLFWwindow* window)
		{
			glfwDestroyWindow(window);
		}
	};

	class GLFW_Window : public IWindow
	{
		friend class GLFW_Loader_OGL;

	public:
		GLFW_Window(const WindowProperties& properties);

	public:
		void PollEvents() override;

	public:
		void Show() override;

		void Hide() override;

	public:
		bool IsOpen() const override;

	public:
		void* GetWindow() const override;

	private:
		std::unique_ptr<GLFWwindow, GLFWWindowDeleter> m_Window;
	};
}