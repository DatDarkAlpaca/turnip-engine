#pragma once
#include <GLFW/glfw3.h>

#include "Common.h"
#include "Core/Event/Event.h"
#include "Core/Window/Window.h"

namespace tur
{
	class Event;

	class GLFWWindowDestroyer
	{
	public:
		void operator() (GLFWwindow* window);
	};

	class WindowGLFW : public Window
	{
	public:
		WindowGLFW(const WindowProperties& properties);

	public:
		void PollEvents() override;

		void SetEventCallback(const FnEventCallback& callback) override;

	public:
		void Hide() override;

		void Show() override;

	public:
		bool IsOpen() const override;

		glm::vec2 GetPosition() const override;

		void SetPosition(const glm::vec2& position) override;

		void SetSizeLimits(const glm::vec2& minimumSize, const glm::vec2& maximumSize) override;

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
}