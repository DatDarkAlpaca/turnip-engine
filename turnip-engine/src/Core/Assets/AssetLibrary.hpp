#pragma once
#include "Core/Assets/TextureAsset.hpp"

namespace tur
{
	class AssetLibrary
	{
	public:
		AssetHandle InsertTexture(const TextureAsset& textureAsset)
		{
			m_Textures.push_back(tur::MakeShared<TextureAsset>(textureAsset));
			return AssetHandle(m_Textures.size() - 1);
		}

		tur_weak<TextureAsset> GetTexture(AssetHandle handle)
		{
			return m_Textures[static_cast<size_t>(handle)];
		}

	private:
		std::vector<tur_shared<TextureAsset>> m_Textures;
	};
}