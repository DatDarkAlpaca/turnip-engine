#pragma once
#include "pch.h"

// Todo: move the definition of mouse buttons to the platform specific layer
namespace tur
{
	enum class MouseButton
	{
		BUTTON_1 = 0,
		BUTTON_2 = 1,
		BUTTON_3 = 2,
		BUTTON_4 = 3,
		BUTTON_5 = 4,
		BUTTON_6 = 5,
		BUTTON_7 = 6,
		BUTTON_8 = 7,
		LEFT     = BUTTON_1,
		RIGHT    = BUTTON_2,
		MIDDLE   = BUTTON_3,
		LAST     = BUTTON_8,
		AMOUNT   = 8
	};
}