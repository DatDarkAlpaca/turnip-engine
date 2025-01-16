#include "pch.hpp"
#include "asset_library.hpp"

#include <stb_image.h>

namespace tur
{
	asset_handle load_texture_asset(AssetLibrary* assetLibrary, const std::filesystem::path& filepath)
	{
		if (!std::filesystem::exists(filepath))
		{
			TUR_LOG_ERROR("Failed to load texture: '{}'. File does not exist.", filepath.string());
			return invalid_handle;
		}

		int width, height, channels;

		DataBuffer buffer;
		{
			buffer.data = stbi_load(filepath.string().c_str(), &width, &height, &channels, 0);
			buffer.size = width * height * channels;
		}

		TextureAsset asset;
		asset.filepath = filepath;

		asset.width = static_cast<uint32_t>(width);
		asset.height = static_cast<uint32_t>(height);
		asset.channels = static_cast<uint32_t>(channels);
		asset.data = buffer;

		return static_cast<asset_handle>(assetLibrary->textures.add(asset));
	}

	asset_handle load_texture_asset_float(AssetLibrary* assetLibrary, const std::filesystem::path& filepath)
	{
		if (!std::filesystem::exists(filepath))
		{
			TUR_LOG_ERROR("Failed to load texture: '{}'. File does not exist.", filepath.string());
			return invalid_handle;
		}

		int width, height, channels;

		DataBuffer buffer;
		{
			buffer.data = stbi_loadf(filepath.string().c_str(), &width, &height, &channels, 0);
			buffer.size = width * height * channels;
		}

		TextureAsset asset;
		asset.filepath = filepath;

		asset.width = static_cast<uint32_t>(width);
		asset.height = static_cast<uint32_t>(height);
		asset.channels = static_cast<uint32_t>(channels);
		asset.data = buffer;
		asset.floatTexture = true;

		return static_cast<asset_handle>(assetLibrary->textures.add(asset));
	}

	void unload_texture_asset(AssetLibrary* assetLibrary, asset_handle handle)
	{
		TextureAsset asset = assetLibrary->textures.remove(handle);
		stbi_image_free(asset.data.data);
	}
}