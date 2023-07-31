#pragma once
#include "pch.h"

// Todo: move the definition of key mods to the platform specific layer
namespace tur
{
	enum class Mods
	{
		SHIFT		= GLFW_MOD_SHIFT,
		CONTROL		= GLFW_MOD_CONTROL,
		ALT			= GLFW_MOD_ALT,
		SUPER		= GLFW_MOD_SUPER,
		CAPS_LOCK	= GLFW_MOD_CAPS_LOCK,
		NUM_LOCK	= GLFW_MOD_NUM_LOCK,
		AMOUNT		= 6
	};
}