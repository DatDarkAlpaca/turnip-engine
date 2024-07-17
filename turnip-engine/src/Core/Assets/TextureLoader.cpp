#include "pch.h"
#include <stb_image.h>

#include "Common.h"
#include "TextureLoader.hpp"

namespace tur
{
	TextureAsset TextureLoader::Load(const AssetMetadata& metadata)
	{
		if (!std::filesystem::exists(metadata.filepath))
		{
			TUR_LOG_ERROR("Failed to load texture: '{}'. File does not exist.", metadata.filepath.string());
			return {};
		}

		int width, height, channels;

		Buffer buffer;
		{
			buffer.data = stbi_load(metadata.filepath.string().c_str(), &width, &height, &channels, 4);
			buffer.size = width * height * channels;
		}

		TextureAsset asset;
		asset.metadata = metadata;

		asset.width = static_cast<uint32_t>(width);
		asset.height = static_cast<uint32_t>(height);
		asset.channels = static_cast<uint32_t>(channels);
		asset.buffer = buffer;

		return asset;
	}
}