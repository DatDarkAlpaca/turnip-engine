#include "pch.hpp"
#include <GLFW/glfw3.h>
#include "platform/opengl_context.hpp"
#include "platform/glfw/window_glfw.hpp"

namespace tur
{
	void initialize_swapbuffer(Swapbuffer* swapbuffer, Window* window)
	{
		swapbuffer->window = window;
	}

	void present_swapbuffer(Swapbuffer* swapbuffer)
	{
		glfwSwapBuffers(swapbuffer->window->window);
	}

	void destroy_swapbuffer(Swapbuffer* swapbuffer)
	{
		// Blank
	}
}