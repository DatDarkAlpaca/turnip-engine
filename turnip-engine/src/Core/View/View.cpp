#include "pch.hpp"
#include "View.hpp"

#include "Core/Engine/TurnipEngine.hpp"

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

