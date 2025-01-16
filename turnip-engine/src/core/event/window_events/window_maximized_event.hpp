#pragma once
#include "core/event/event.hpp"

namespace tur
{
	class WindowMaximizedEvent : public Event
	{
		DEFINE_EVENT(EventType::WINDOW_MAXIMIZED);

	public:
		WindowMaximizedEvent(int maximizationState)
			: maximizationState(maximizationState)
		{

		}

	public:
		int maximizationState;
	};
}