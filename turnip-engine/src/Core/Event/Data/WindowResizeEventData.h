#pragma once
#include "BaseEventData.h"
#include "Core/Event/EventType.h"

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