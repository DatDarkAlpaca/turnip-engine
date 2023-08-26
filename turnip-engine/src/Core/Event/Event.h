#pragma once
#include <memory>
#include "EventType.h"
#include "Data/BaseEventData.h"

namespace tur
{
	class Event
	{
		friend class Subscriber;

	public:
		Event(EventType type, std::unique_ptr<BaseEventData> data)
			: type(type)
			, data(std::move(data))
		{ }

	public:
		EventType type = EventType::UNKNOWN;
		std::unique_ptr<BaseEventData> data;
		bool handled = false;
	};
}