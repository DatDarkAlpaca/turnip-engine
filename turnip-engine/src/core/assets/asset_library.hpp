#pragma once
#include <filesystem>

#include "common.hpp"
#include "texture_asset.hpp"
#include "core/free_list.hpp"

namespace tur
{
	using asset_handle = handle_type;

	struct AssetLibrary
	{
		free_list<TextureAsset> textures;
		std::unordered_map<std::string, asset_handle> assetFilepathMap;
	};

	struct AssetInformation
	{
		asset_handle handle;
		bool isDuplicate = false;
	};

	AssetInformation load_texture_asset(AssetLibrary* assetLibrary, const std::filesystem::path& filepath);

	AssetInformation load_texture_asset_float(AssetLibrary* assetLibrary, const std::filesystem::path& filepath);

	void unload_texture_asset(AssetLibrary* assetLibrary, asset_handle handle);
}