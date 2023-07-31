#pragma once
#ifdef TUR_WINDOWING_GLFW

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
		void SetEventCallback(const FnEventCallback& eventCallback) override;

		void PollEvents() override;

	public:
		void Show() override;

		void Hide() override;

	public:
		bool IsOpen() const override;

	public:
		void* GetWindow() const override;

	private:
		void SetupCallbacks();

	private:
		std::unique_ptr<GLFWwindow, GLFWWindowDeleter> m_Window;

		struct WindowData
		{
			FnEventCallback eventCallback;
		} m_WindowData;
	};

	using Window = GLFW_Window;
}
#endif // TUR_WINDOWING_GLFW