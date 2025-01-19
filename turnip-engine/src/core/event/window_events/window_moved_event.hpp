#pragma once
#include "core/event/event.hpp"

namespace tur
{
	struct WindowMovedEvent : public Event
	{
		DEFINE_EVENT(EventType::WINDOW_MOVED);

	public:
		WindowMovedEvent(u32 x, u32 y)
			: x(x)
			, y(y)
		{

		}

	public:
		u32 x = 0, y = 0;
	};
}