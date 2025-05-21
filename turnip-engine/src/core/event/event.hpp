#pragma once
#include "event_type.hpp"

namespace tur
{ 
	using event_id = u64;

	#define DEFINE_EVENT(ID)																		\
		public:																						\
			static inline event_id get_id() { return static_cast<event_id>(ID); }					\
			event_id id() const override { return static_cast<event_id>(ID); };

	class Event
	{
	public:
		virtual ~Event() = default;

	public:
		virtual event_id id() const { return static_cast<event_id>(EventType::UNKNOWN); }

	public:
		bool handled = false;
	};

	using EventCallback = std::function<void(Event&)>;
}