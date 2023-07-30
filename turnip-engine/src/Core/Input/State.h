#pragma once
#include "pch.h"

// Todo: move the definition of states to the platform specific layer
namespace tur
{
	enum class State
	{
		RELEASE = GLFW_RELEASE,
		PRESS   = GLFW_PRESS,
		REPEAT  = GLFW_REPEAT
	};
}