#pragma once
#include <vector>
#include <limits>
#include <numeric>

#include "Rendering/Type/Types.h"
#undef max

namespace tur
{
	struct VertexAttribute
	{
	public:
		static constexpr uint32_t Auto = std::numeric_limits<uint32_t>::max();

	public:
		uint32_t location;
		Format layoutFormat;
		uint32_t offset = Auto;
	};

	struct VertexFormat
	{
		static constexpr uint32_t Auto = std::numeric_limits<uint32_t>::max();

	public:
		VertexAttribute& Add(const VertexAttribute& attribute)
		{
			attributes.push_back(attribute);
			return attributes[attributes.size() - 1];
		}

	public:
		std::vector<VertexAttribute> attributes;
		uint32_t stride = Auto;
	};
}