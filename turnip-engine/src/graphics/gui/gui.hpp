#pragma once

namespace tur
{
	struct Window;

	void initialize_gui(Window* window);
	extern void initialize_gui_graphics(Window* window);

	void begin_gui();
	void end_gui();
}