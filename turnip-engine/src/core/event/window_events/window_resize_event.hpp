#pragma once
#include "core/event/event.hpp"

namespace tur
{
	struct WindowResizeEvent : public Event
	{
		DEFINE_EVENT(EventType::WINDOW_RESIZE);

	public:
		WindowResizeEvent(u32 width, u32 height)
			: width(width)
			, height(height)
		{

		}

	public:
		u32 width = 0, height = 0;
	};
}