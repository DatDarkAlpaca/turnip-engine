#pragma once
#include "Core/Event/Event.hpp"

namespace tur
{
	enum class MouseButton
	{
		MOUSE_BUTTON_UNKNOWN,
		MOUSE_BUTTON_1,
		MOUSE_BUTTON_2,
		MOUSE_BUTTON_3,
		MOUSE_BUTTON_4,
		MOUSE_BUTTON_5,
		MOUSE_BUTTON_6,
		MOUSE_BUTTON_7,
		MOUSE_BUTTON_8
	};

	inline constexpr const char* GetMouseButtonName(MouseButton button)
	{
		switch (button)
		{
			case MouseButton::MOUSE_BUTTON_UNKNOWN:
				return "MOUSE_BUTTON_UNKNOWN";

			case MouseButton::MOUSE_BUTTON_1:
				return "MOUSE_BUTTON_1";

			case MouseButton::MOUSE_BUTTON_2:
				return "MOUSE_BUTTON_2";

			case MouseButton::MOUSE_BUTTON_3:
				return "MOUSE_BUTTON_3";

			case MouseButton::MOUSE_BUTTON_4:
				return "MOUSE_BUTTON_4";

			case MouseButton::MOUSE_BUTTON_5:
				return "MOUSE_BUTTON_5";

			case MouseButton::MOUSE_BUTTON_6:
				return "MOUSE_BUTTON_6";

			case MouseButton::MOUSE_BUTTON_7:
				return "MOUSE_BUTTON_7";

			case MouseButton::MOUSE_BUTTON_8:
				return "MOUSE_BUTTON_8";
		}
	}
}