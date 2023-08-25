#pragma once
#include "Common.h"

namespace tur
{
	enum class EventType : U64
	{
		UNKNOWN = 0,

		WINDOW_CLOSE,
		WINDOW_RESIZE,
	};
}