#pragma once
#include "pch.h"

namespace tur
{
	class Window
	{
	public:
		void Initialize(int width, int height, const char* title);

		void Destroy();

	public:
		bool ShouldClose() const { return glfwWindowShouldClose(m_Window); }

		void PollEvents() const { glfwPollEvents(); }

		void SwapBuffers() const { glfwSwapBuffers(m_Window); }

	public:
		inline void SetViewport(const glm::vec2& viewport) { glViewport(0, 0, (U32)viewport.x, (U32)viewport.y); }

		inline GLFWwindow* Get() const { return m_Window; }

	private:
		GLFWwindow* m_Window = nullptr;
	};
}
