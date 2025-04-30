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
		// TODO: URGENT - Use filepath to determine if texture is already loaded
		free_list<TextureAsset> textures;
	};

	asset_handle load_texture_asset(AssetLibrary* assetLibrary, const std::filesystem::path& filepath);

	asset_handle load_texture_asset_float(AssetLibrary* assetLibrary, const std::filesystem::path& filepath);

	void unload_texture_asset(AssetLibrary* assetLibrary, asset_handle handle);
}