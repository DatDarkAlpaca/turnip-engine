#pragma once
#include "pch.h"
#include "IEvent.h"

namespace tur
{
	class WindowResizeEvent : public IEvent
	{
	public:
		WindowResizeEvent(int width, int height)
			: width(width), height(height) { }

	public:
		USE_EVENT_TYPE(WindowResize);

	public:
		int width = 0, height = 0;
	};
}