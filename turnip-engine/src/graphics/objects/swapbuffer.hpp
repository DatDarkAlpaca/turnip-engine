#pragma once
#include "platform/window.hpp"

namespace tur
{
	struct Swapbuffer;

	extern void initialize_swapbuffer(Swapbuffer* swapbuffer, Window* window);

	extern void present_swapbuffer(Swapbuffer* swapbuffer);

	extern void destroy_swapbuffer(Swapbuffer* swapbuffer);
}