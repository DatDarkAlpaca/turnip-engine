#pragma once
#include "pch.h"

namespace tur
{
	enum class State
	{
		RELEASE = GLFW_RELEASE,
		PRESS   = GLFW_PRESS,
		REPEAT  = GLFW_REPEAT
	};
}