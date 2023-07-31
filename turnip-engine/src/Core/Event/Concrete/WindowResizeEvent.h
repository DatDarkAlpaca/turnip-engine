#pragma once
#include "pch.h"
#include "Core/Event/IEvent.h"

namespace tur
{
	class WindowResizeEvent : public IEvent
	{
	public:
		WindowResizeEvent(int width, int height)
			: width(width), height(height) { }

	public:
		USE_EVENT_TYPE(WINDOW_RESIZE);

	public:
		int width = 0, height = 0;
	};
}