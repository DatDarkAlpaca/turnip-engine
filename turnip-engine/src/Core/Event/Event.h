#pragma once
#include "EventType.h"

namespace tur
{ 
	#define DEFINE_EVENT(TYPE)											   \
		public:															   \
			static inline EventType GetEventType() { return TYPE; }		   \
			static inline EventType s_EventType = TYPE;					   \
			virtual EventType type() const { return TYPE; }

	class Event
	{
	public:
		virtual ~Event() = default;

	public:
		virtual EventType type() const { return EventType::UNKNOWN; }

	public:
		bool handled = false;
	};

	using FnEventCallback = std::function<void(Event&)>;
}