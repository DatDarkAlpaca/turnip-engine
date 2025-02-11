#pragma once
#include <filesystem>
#include "core/buffer.hpp"
#include "graphics/objects/texture.hpp"

namespace tur
{
	struct TextureAsset
	{
		std::filesystem::path filepath;
        TextureDataFormat dataFormat  = TextureDataFormat::RGBA;
		DataBuffer data;

		u32 width    = 0;
		u32 height   = 0;
		u32 channels = 0;

		bool floatTexture = false;
	};
}

namespace tur
{
    inline std::pair<TextureDescriptor, TextureAsset> create_default_texture()
    {
        TextureDescriptor descriptor;
        {
            descriptor.format = TextureFormat::RGBA;
            descriptor.type = TextureType::TEXTURE_2D;
            descriptor.width = 2;
            descriptor.height = 2;
        }

        u8 DefaultTextureData[] = {
            210, 210, 210, 255,		 240, 240, 240, 255,
            240, 240, 240, 255,		 210, 210, 210, 255,
        };

        TextureAsset asset;
        {
            asset.data.data = DefaultTextureData;
            asset.data.size = sizeof(DefaultTextureData);

            asset.width = 2;
            asset.height = 2;
            asset.channels = 4;
        }

        return { descriptor, asset };
    }
}