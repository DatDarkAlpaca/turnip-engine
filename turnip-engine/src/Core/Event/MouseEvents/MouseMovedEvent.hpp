#pragma once
#include "MouseEvent.hpp"

namespace tur
{
	struct MouseMovedEvent : public Event
	{
		DEFINE_EVENT(EventType::MOUSE_MOVE);

	public:
		MouseMovedEvent(double x, double y)
			: x(x)
			, y(y)
		{

		}

	public:
		double x = 0, y = 0;
	};
}