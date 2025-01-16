#pragma once
#include "core/event/event.hpp"

namespace tur
{
	class WindowCloseEvent : public Event
	{
		DEFINE_EVENT(EventType::WINDOW_CLOSE);
	};
}