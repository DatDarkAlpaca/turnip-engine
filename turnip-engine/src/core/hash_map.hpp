#pragma once
#include "common.hpp"

namespace tur
{
	class SpatialHashMap2D
	{
	public:
		explicit SpatialHashMap2D(float cellSize, u32 tableSize)
			: m_CellSize(cellSize)
		{

		}

	public:
		void insert(const glm::vec2 position)
		{
			int hash = (static_cast<u32>(position.x) * 92837111) ^ (static_cast<u32>(position.y) * 689287499);
			// add to unordered map
		}

	private:
		u32 hash_coordinate(const glm::vec2 position) const
		{
			u32 hash = (static_cast<u32>(position.x) * 92837111) ^ (static_cast<u32>(position.y) * 689287499);
			return std::labs(hash);
		}

	private:
		float m_CellSize = 0.0f;
	};
}