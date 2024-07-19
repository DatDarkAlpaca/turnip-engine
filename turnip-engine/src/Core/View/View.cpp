#include "pch.h"
#include "View.h"

#include "Core/Engine/TurnipEngine.h"

namespace tur
{
	AssetLibrary& tur::View::AssetLibrary() const
	{
		return engine->GetAssetLibrary();
	}

	WorkerPool& tur::View::WorkerPool() const
	{
		return engine->GetWorkerPool();
	}
}

