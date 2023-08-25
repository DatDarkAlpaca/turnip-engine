#pragma once
#include "Core/Event/EventType.h"
#include "BaseEventData.h"

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