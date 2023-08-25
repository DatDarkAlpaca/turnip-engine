#pragma once
#include "Core/Event/EventType.h"
#include "BaseEventData.h"

namespace tur
{
	class WindowResizeEventData : public BaseEventData
	{
	public:
		WindowResizeEventData(unsigned width, unsigned height)
			: width(width)
			, height(height)
		{

		}

	public:
		static constexpr inline EventType type = EventType::WINDOW_RESIZE;
		unsigned width, height;
	};
}