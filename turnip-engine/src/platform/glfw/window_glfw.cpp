#include "pch.hpp"
#include "window_glfw.hpp"
#include "events_glfw.hpp"

#include "core/event/events.hpp"
#include "core/logger/logger.hpp"

namespace tur
{
	static void GLFWErrorCallback(int errorCode, const char* description)
	{
		TUR_LOG_ERROR("[GLFW]: {} [{}]", description, errorCode);
	}

	static void set_window_events(Window* window)
	{
		// Status:
		glfwSetWindowCloseCallback(window->window, [](GLFWwindow* window) {
			auto* data = static_cast<Window::Data*>(glfwGetWindowUserPointer(window));

			WindowCloseEvent event;
			data->eventCallback(event);
		});

		glfwSetWindowMaximizeCallback(window->window, [](GLFWwindow* window, int maximized) {
			auto* data = static_cast<Window::Data*>(glfwGetWindowUserPointer(window));

			WindowMaximizedEvent event(maximized);
			data->eventCallback(event);
		});

		glfwSetWindowFocusCallback(window->window, [](GLFWwindow* window, int focus) {
			auto* data = static_cast<Window::Data*>(glfwGetWindowUserPointer(window));

			WindowFocusEvent event(focus);
			data->eventCallback(event);
		});

		// Size:
		glfwSetWindowSizeCallback(window->window, [](GLFWwindow* window, int width, int height) {
			auto* data = static_cast<Window::Data*>(glfwGetWindowUserPointer(window));

			WindowResizeEvent event(width, height);
			data->eventCallback(event);
		});

		glfwSetFramebufferSizeCallback(window->window, [](GLFWwindow* window, int width, int height) {
			auto* data = static_cast<Window::Data*>(glfwGetWindowUserPointer(window));

			WindowFramebufferEvent event(width, height);
			data->eventCallback(event);
		});

		glfwSetWindowContentScaleCallback(window->window, [](GLFWwindow* window, float xScale, float yScale) {
			auto* data = static_cast<Window::Data*>(glfwGetWindowUserPointer(window));

			WindowContentScaleEvent event(xScale, yScale);
			data->eventCallback(event);
		});

		// Position:
		glfwSetWindowPosCallback(window->window, [](GLFWwindow* window, int xPos, int yPos) {
			auto* data = static_cast<Window::Data*>(glfwGetWindowUserPointer(window));

			WindowMovedEvent event(xPos, yPos);
			data->eventCallback(event);
		});

		// Keyboard:
		glfwSetKeyCallback(window->window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			auto* data = static_cast<Window::Data*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
				case GLFW_PRESS:
				{
					Key convertedKey = translate_key(key);
					KeyboardMods convertedMods = translate_key_mods(mods);

					KeyPressedEvent event(convertedKey, convertedMods);
					data->eventCallback(event);
				} break;

				case GLFW_RELEASE:
				{
					Key convertedKey = translate_key(key);
					KeyboardMods convertedMods = translate_key_mods(mods);

					KeyReleasedEvent event(convertedKey, convertedMods);
					data->eventCallback(event);
				} break;

				case GLFW_REPEAT:
				{
					Key convertedKey = translate_key(key);
					KeyboardMods convertedMods = translate_key_mods(mods);

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
		glfwSetCursorPosCallback(window->window, [](GLFWwindow* window, double x, double y) {
			auto* data = static_cast<Window::Data*>(glfwGetWindowUserPointer(window));

			MouseMovedEvent event(x, y);
			data->eventCallback(event);
		});

		glfwSetMouseButtonCallback(window->window, [](GLFWwindow* window, int button, int action, int mods) {
			auto* data = static_cast<Window::Data*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButton convertedButton = translate_mouse_button(button);
					MousePressedEvent event(convertedButton);
					data->eventCallback(event);
				} break;

				case GLFW_RELEASE:
				{
					MouseButton convertedButton = translate_mouse_button(button);
					MouseReleasedEvent event(convertedButton);
					data->eventCallback(event);
				} break;

				default:
				{
					TUR_LOG_WARN("Invalid keyboard action passed to window callback: {}", action);
				} break;
			}
		});

		glfwSetScrollCallback(window->window, [](GLFWwindow* window, double xOffset, double yOffset) {
			auto* data = static_cast<Window::Data*>(glfwGetWindowUserPointer(window));

			MouseScrollEvent event(xOffset, yOffset);
			data->eventCallback(event);
		});
	}
}

namespace tur
{
	void initialize_windowing_system()
	{
		glfwSetErrorCallback(GLFWErrorCallback);

		if (!glfwInit())
			TUR_LOG_CRITICAL("Failed to initialize the platform system.");
	}

	void shutdown_windowing_system()
	{
		glfwTerminate();
	}

	void initialize_window(Window* window, const WindowProperties& properties)
	{
		window->window = glfwCreateWindow(
			properties.dimensions.x,
			properties.dimensions.y,
			properties.title.c_str(),
			nullptr,
			nullptr
		);

		window->properties = properties;

		if (!window->window)
		{
			TUR_LOG_CRITICAL("Failed to create a valid GLFW window. GLFW is probably uninitialized");
			return;
		}

		glfwSetWindowUserPointer(window->window, &window->data);

		set_properties_window(window, properties);
		set_window_events(window);
	}

	void set_callback_window(Window* window, EventCallback&& callback)
	{
		window->data.eventCallback = callback;
	}

	void poll_events(Window* window)
	{
		glfwPollEvents();
	}

	void set_properties_window(Window* window, const WindowProperties& properties)
	{
		auto* windowGlfw = window->window;

		// Current Size:
		glfwSetWindowSize(windowGlfw, (int)properties.dimensions.x, (int)properties.dimensions.y);

		// Size Limits:
		glfwSetWindowSizeLimits(
			windowGlfw,
			(int)properties.minSize.x,
			(int)properties.minSize.y,
			(int)properties.maxSize.x,
			(int)properties.maxSize.y
		);

		// Position:
		if (properties.position != invalid_size)
			glfwSetWindowPos(windowGlfw, properties.position.x, properties.position.y);

		// Title:
		glfwSetWindowTitle(windowGlfw, properties.title.c_str());
	}

	void shutdown_window(Window* window)
	{
		glfwDestroyWindow(window->window);
	}

	bool is_open_window(Window* window)
	{
		return !glfwWindowShouldClose(window->window);
	}

	void show_window(Window* window)
	{
		glfwShowWindow(window->window);
	}

	void hide_window(Window* window)
	{
		glfwHideWindow(window->window);
	}
}