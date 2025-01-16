#pragma once
#include "core/event/event.hpp"

namespace tur
{
	struct Window;

	extern void initialize_windowing_system();

	extern void shutdown_windowing_system();


	extern void initialize_window(Window* window);

	extern void set_callback_window(Window* window, EventCallback&& callback);

	extern void set_properties_window(Window* window);

	extern void poll_events(Window* window);

	extern void shutdown_window(Window* window);

	extern bool is_open_window(Window* window);

	extern void show_window(Window* window);

	extern void hide_window(Window* window);
}