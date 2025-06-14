#pragma once

namespace tur
{
	using asset_handle = handle_type;

	enum class AssetType
	{
		NONE = 0,
		TEXTURE
	};

	static inline constexpr const char* get_asset_type_name(AssetType type)
	{
		if (type == AssetType::NONE)
			return "None";

		if (type == AssetType::TEXTURE)
			return "Texture";
	}
}