#pragma once
#include "core/event/event.hpp"

namespace tur
{
	struct WindowFramebufferEvent : public Event
	{
		DEFINE_EVENT(EventType::WINDOW_FRAMEBUFFER_RESIZE);

	public:
		WindowFramebufferEvent(u32 width, u32 height)
			: width(width)
			, height(height)
		{

		}

	public:
		u32 width = 0, height = 0;
	};
}