#pragma once
#include "MouseEvent.h"

namespace tur
{
	struct MouseScrollEvent : public Event
	{
		DEFINE_EVENT(EventType::MOUSE_SCROLL);

	public:
		MouseScrollEvent(double xOffset, double yOffset)
			: xOffset(xOffset)
			, yOffset(yOffset)
		{

		}

	public:
		double xOffset = 0, yOffset = 0;
	};
}