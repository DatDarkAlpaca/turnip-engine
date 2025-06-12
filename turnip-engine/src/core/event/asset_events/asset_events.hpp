#pragma once
#include "core/event/event.hpp"
#include "core/assets/asset_library.hpp"

namespace tur
{
	class OnNewTextureLoad : public Event
	{
		DEFINE_EVENT(EventType::ASSET_ON_LOAD_NEW_TEXTURE);

	public:
		OnNewTextureLoad(asset_handle assetHandle)
			: assetHandle(assetHandle)
		{

		}

	public:
		asset_handle assetHandle = invalid_handle;
	};
}