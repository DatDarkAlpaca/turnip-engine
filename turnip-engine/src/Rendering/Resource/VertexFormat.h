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
		uint32_t location;
		Format layoutFormat;
		uint32_t stride;
		uint32_t offset;
	};

	struct VertexFormat
	{
	public:
		VertexAttribute& Add(const VertexAttribute& attribute)
		{
			attributes.push_back(attribute);
			return attributes[attributes.size() - 1];
		}

	public:
		std::vector<VertexAttribute> attributes;
	};
}