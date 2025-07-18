#pragma once
#include <filesystem>
#include "common.hpp"

#include "asset.hpp"
#include "texture_asset.hpp"
#include "core/free_list.hpp"
#include "utils/uuid/uuid.hpp"

namespace tur
{
	struct AssetLibrary
	{
		free_list<TextureAsset> textures;
		std::unordered_map<std::string, asset_handle> assetFilepathMap;
	};

	struct AssetInformation
	{
	public:
		bool is_valid() const { return assetHandle != invalid_handle && !isDuplicate; }

	public:
		asset_handle assetHandle = invalid_handle;
		bool isDuplicate = false;
	};

	AssetInformation load_texture_asset(AssetLibrary* assetLibrary, const std::filesystem::path& filepath, const UUID& uuid = invalid_uuid);

	AssetInformation load_texture_asset_float(AssetLibrary* assetLibrary, const std::filesystem::path& filepath, const UUID& uuid = invalid_uuid);

	void unload_texture_asset(AssetLibrary* assetLibrary, asset_handle assetHandle);
}