#pragma once
#include <filesystem>
#include "core/buffer.hpp"

namespace tur
{
	struct TextureAsset
	{
		std::filesystem::path filepath;
		DataBuffer data;

		uint32_t width    = 0;
		uint32_t height   = 0;
		uint32_t channels = 0;

		bool floatTexture = false;
	};
}