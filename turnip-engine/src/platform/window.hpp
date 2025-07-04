#pragma once
#include "core/event/event.hpp"

namespace tur
{
	struct WindowProperties
	{
		enum Position : int { DEFAULT = -1 };

	public:
		std::string title		= "TurnipEngine v1.0";
		glm::uvec2 dimensions	= glm::vec2(640, 480);
		glm::uvec2 position		= glm::vec2(Position::DEFAULT, Position::DEFAULT);
		glm::uvec2 minSize		= glm::vec2(0);
		glm::uvec2 maxSize		= dimensions;
		bool startsMaximized	= false;
	};

	struct Window;

	extern void initialize_windowing_system();
	extern void shutdown_windowing_system();

	extern void initialize_window(Window* window, const WindowProperties& properties);
	extern void set_callback_window(Window* window, EventCallback&& callback);
	extern void poll_events(Window* window);

	extern void set_properties_window(Window* window, const WindowProperties& properties);
	extern void set_window_title(Window* window, const std::string& name);

	extern void shutdown_window(Window* window);
	extern bool is_open_window(Window* window);

	extern void show_window(Window* window);
	extern void hide_window(Window* window);
	extern void maximimize_window(Window* window);

	extern glm::vec2  get_mouse_cursor_position_window(Window* window);

	extern glm::uvec2 get_window_size(Window* window);
}