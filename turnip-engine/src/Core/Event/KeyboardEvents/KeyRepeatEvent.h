#pragma once
#include "KeyboardEvent.h"

namespace tur
{
	struct KeyRepeatEvent : public Event
	{
		DEFINE_EVENT(EventType::KEY_REPEAT);

	public:
		KeyRepeatEvent(Key key, KeyboardMods mods)
			: key(key)
			, mods(mods)
		{

		}

	public:
		Key key;
		KeyboardMods mods;
	};
}