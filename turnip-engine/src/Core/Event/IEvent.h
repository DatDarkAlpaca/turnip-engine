#pragma once
#include "EventType.h"

namespace tur
{
	class IEvent
	{
		friend class Subscriber;

	public:
		IEvent() = default;
		virtual ~IEvent() = default;

	public:
		virtual EventType GetType() const = 0;

	protected:
		bool m_Handled = false;
	};
}

#define USE_EVENT_TYPE(type)									\
	static EventType Type() { return EventType::type; }			\
	EventType GetType() const final { return EventType::type; }
