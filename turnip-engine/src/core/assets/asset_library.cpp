#include "pch.hpp"
#include "asset_library.hpp"

#include <stb_image.h>

namespace tur
{
	AssetInformation load_texture_asset(AssetLibrary* assetLibrary, const std::filesystem::path& filepath)
	{
		if (!std::filesystem::exists(filepath))
		{
			TUR_LOG_ERROR("Failed to load texture: '{}'. File does not exist.", filepath.string());
			return { invalid_handle };
		}

		if (assetLibrary->assetFilepathMap.find(filepath.string()) != assetLibrary->assetFilepathMap.end())
			return { assetLibrary->assetFilepathMap[filepath.string()], true };

		int width, height, channels;

		DataBuffer buffer;
		{
			buffer.data = stbi_load(filepath.string().c_str(), &width, &height, &channels, 0);
			buffer.size = width * height * channels;
		}

		TextureAsset asset;
		asset.filepath = filepath;

		asset.width = static_cast<u32>(width);
		asset.height = static_cast<u32>(height);
		asset.channels = static_cast<u32>(channels);
		asset.data = buffer;

		asset_handle handle = static_cast<asset_handle>(assetLibrary->textures.add(asset));
		assetLibrary->assetFilepathMap[filepath.string()] = handle;

		return { handle };
	}

	AssetInformation load_texture_asset_float(AssetLibrary* assetLibrary, const std::filesystem::path& filepath)
	{
		if (!std::filesystem::exists(filepath))
		{
			TUR_LOG_ERROR("Failed to load texture: '{}'. File does not exist.", filepath.string());
			return { invalid_handle };
		}

		if (assetLibrary->assetFilepathMap.find(filepath.string()) != assetLibrary->assetFilepathMap.end())
			return { assetLibrary->assetFilepathMap[filepath.string()], true };

		int width, height, channels;

		DataBuffer buffer;
		{
			buffer.data = stbi_loadf(filepath.string().c_str(), &width, &height, &channels, 0);
			buffer.size = width * height * channels;
		}

		TextureAsset asset;
		asset.filepath = filepath;

		asset.width = static_cast<u32>(width);
		asset.height = static_cast<u32>(height);
		asset.channels = static_cast<u32>(channels);
		asset.data = buffer;
		asset.floatTexture = true;

		asset_handle handle = static_cast<asset_handle>(assetLibrary->textures.add(asset));
		assetLibrary->assetFilepathMap[filepath.string()] = handle;

		return { handle };
	}

	void unload_texture_asset(AssetLibrary* assetLibrary, asset_handle handle)
	{
		// TODO: remove from texturefilepathmap

		TextureAsset asset = assetLibrary->textures.remove(handle);
		stbi_image_free(asset.data.data);
	}
}