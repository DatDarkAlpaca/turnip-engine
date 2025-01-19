#pragma once
#include <filesystem>
#include "core/buffer.hpp"

namespace tur
{
	struct TextureAsset
	{
		std::filesystem::path filepath;
		DataBuffer data;

		u32 width    = 0;
		u32 height   = 0;
		u32 channels = 0;

		bool floatTexture = false;
	};
}