#pragma once
#include "TextureAsset.hpp"

namespace tur
{
	class TextureLoader
	{
	public:
		static TextureAsset Load(const AssetMetadata& metadata);
	};
}