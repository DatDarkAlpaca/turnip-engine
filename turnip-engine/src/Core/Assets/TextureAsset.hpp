#pragma once
#include "Core/Buffer/Buffer.hpp"
#include "Assets.hpp"

namespace tur
{
	struct TextureAsset
	{
		AssetMetadata metadata;
		Buffer buffer;

		uint32_t width = 0;
		uint32_t height = 0;
		uint32_t channels = 0;
	};
}