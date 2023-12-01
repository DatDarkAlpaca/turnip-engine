#pragma once
#include "Core/View/ViewHolder.h"
#include "Graphics/IGraphicsBackend.h"

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
		tur_unique<IGraphicsBackend> backend = nullptr;

	public:
		bool initialized = false;
	};
}