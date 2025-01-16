#pragma once
#include "core/event/event.hpp"

namespace tur
{
	struct WindowContentScaleEvent : public Event
	{
		DEFINE_EVENT(EventType::WINDOW_CONTENT_SCALE);

	public:
		WindowContentScaleEvent(float width, float height)
			: width(width)
			, height(height)
		{

		}

	public:
		float width = 1.0, height = 1.0;
	};
}