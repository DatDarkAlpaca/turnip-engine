#pragma once
#include "UUID/UUID.h"

namespace tur
{
	template<typename AssetType>
	class AssetLibrary
	{
	public:
		std::shared_ptr<AssetType> LoadAsset(const char* assetName, std::shared_ptr<AssetType> asset)
		{
			TUR_WRAP_DEBUG(
				if (ExistsAsset(assetName))
					TUR_CORE_WARN("Asset overlap found. Overriding asset: {}", assetName);
			)

			m_Assets[assetName] = asset;
			return m_Assets[assetName];
		}

		void UnloadAsset(const char* assetName)
		{
			if (!ExistsAsset(assetName))
			{
				TUR_CORE_ERROR("Failed to delete asset named: {}. No asset with this name exists.", assetName);
				return;
			}

			auto it = m_Assets.find(assetName);

			size_t useCount = it->second.use_count();
			m_Assets.erase(it);

			if (useCount > 1)
				TUR_CORE_WARN("Asset {} persists after being deleted. Current use count: {}", useCount);
		}

		void Clear()
		{
			m_Assets.clear();
		}

	public:
		std::shared_ptr<AssetType> Get(const char* assetName)
		{
			if (!ExistsAsset(assetName))
			{
				TUR_CORE_ERROR("Failed to find asset named: {}", assetName);
				return nullptr;
			}

			return m_Assets[assetName];
		}

		bool ExistsAsset(const char* assetName) const
		{
			return m_Assets.find(assetName) != m_Assets.end();
		}

	public:
		std::unordered_map<std::string_view, std::shared_ptr<AssetType>> m_Assets;
	};
}