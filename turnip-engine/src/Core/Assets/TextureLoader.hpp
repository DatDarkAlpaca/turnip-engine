#pragma once
#include <filesystem>
#include "TextureAsset.hpp"

namespace tur
{
	class TextureLoader
	{
	public:
		static TextureAsset Load(const std::filesystem::path& filepath);
	};
}