#pragma once
#include "Core/View/ViewHolder.h"
#include "Graphics/Graphics.h"

#include "Platform/Platform.h"

namespace tur
{
	class TurnipEngineData
	{
	public:
		TurnipEngineData() = default;

	public:
		tur_unique<Window> window = nullptr;
		tur_unique<ViewHolder> viewHolder = nullptr;

	public:
		bool initialized = false;
	};
}