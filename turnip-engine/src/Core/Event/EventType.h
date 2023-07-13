#pragma once

namespace tur
{
	enum class EventType
	{
		UNKNOWN = 0,
		WINDOW_RESIZE,
		KEYBOARD,
		MOUSE_POSITION,
		MOUSE_BUTTON,
		MOUSE_SCROLL
	};
}