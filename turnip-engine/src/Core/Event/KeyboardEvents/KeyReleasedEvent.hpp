#pragma once
#include "KeyboardEvent.hpp"

namespace tur
{
	struct KeyReleasedEvent : public Event
	{
		DEFINE_EVENT(EventType::KEY_RELEASED);

	public:
		KeyReleasedEvent(Key key, KeyboardMods mods)
			: key(key)
			, mods(mods)
		{

		}

	public:
		Key key;
		KeyboardMods mods;
	};
}