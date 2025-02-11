#pragma once
#include "mouse_event.hpp"

namespace tur
{
	struct MousePressedEvent : public Event
	{
		DEFINE_EVENT(EventType::MOUSE_PRESSED);

	public:
		MousePressedEvent(MouseButton button)
			: button(button)
		{

		}

	public:
		MouseButton button;
	};
}