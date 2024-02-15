#pragma once
#include <numeric>
#include "Rendering/Type/LayoutTypes.h"

namespace tur
{
	struct InputLayoutElement
	{
		uint32_t location;
		uint32_t componentAmount;
		LayoutType valueType;
		bool isNormalized;
	};
}