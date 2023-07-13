#pragma once
#include "pch.h"
#include "IEvent.h"

namespace tur
{
	class MousePositionEvent : public IEvent
	{
	public:
		MousePositionEvent(double xPos, double yPos)
			: position{ xPos, yPos } { }

	public:
		USE_EVENT_TYPE(MOUSE_POSITION);

	public:
		glm::vec2 position;
	};

	class MouseButtonEvent : public IEvent
	{
	public:
		MouseButtonEvent(int button, int action, int mods)
			: button(button), action(action), mods(mods) { }

	public:
		USE_EVENT_TYPE(MOUSE_BUTTON);

	public:
		int button, action, mods;
	};

	class MouseScrollEvent : public IEvent
	{
	public:
		MouseScrollEvent(double offsetX, double offsetY)
			: offsetX(offsetX), offsetY(offsetY) { }

	public:
		USE_EVENT_TYPE(MOUSE_SCROLL);

	public:
		double offsetX, offsetY;
	};
}