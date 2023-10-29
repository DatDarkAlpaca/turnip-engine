#pragma once
#include "KeyboardEvent.h"

namespace tur
{
	struct KeyPressedEvent : public Event
	{
		DEFINE_EVENT(EventType::KEY_PRESSED);

	public:
		KeyPressedEvent(Key key, KeyboardMods mods)
			: key(key)
			, mods(mods)
		{

		}

	public:
		Key key;
		KeyboardMods mods;
	};
}