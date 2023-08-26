#pragma once
#include "BaseEventData.h"
#include "Core/Event/EventType.h"

namespace tur
{
	class WindowCloseEventData : public BaseEventData
	{
	public:
		WindowCloseEventData() = default;

	public:
		static constexpr inline EventType type = EventType::WINDOW_CLOSE;
	};
}