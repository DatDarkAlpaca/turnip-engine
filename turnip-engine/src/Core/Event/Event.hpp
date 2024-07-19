#pragma once
#include "EventType.hpp"

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

	using EventCallback = std::function<void(Event&)>;
}