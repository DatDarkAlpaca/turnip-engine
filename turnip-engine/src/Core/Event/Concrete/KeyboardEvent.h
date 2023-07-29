#pragma once
#include "pch.h"
#include "Core/Event/IEvent.h"

namespace tur
{
	class KeyboardEvent : public IEvent
	{
	public:
		KeyboardEvent(int key, int scancode, int action, int mods)
			: key(key), scancode(scancode), action(action), mods(mods) { }

	public:
		USE_EVENT_TYPE(KEYBOARD);

	public:
		int key, scancode, action, mods;
	};
}