#pragma once
#include "Core/Event/Event.h"

namespace tur
{
	class WindowCloseEvent : public Event
	{
		DEFINE_EVENT(EventType::WINDOW_CLOSE);
	};
}