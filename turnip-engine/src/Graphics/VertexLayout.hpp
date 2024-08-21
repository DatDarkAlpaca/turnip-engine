#pragma once
#include <numeric>
#include "CommonGraphics.hpp"

namespace tur
{
	struct VertexLayout
	{
	public:
		constexpr static inline uint32_t AUTO = std::numeric_limits<uint32_t>::max();

	public:
		struct Attribute
		{
			uint32_t binding;
			uint32_t location;
			uint32_t size;
			DataType type;
			bool normalized = false;
			uint32_t stride = AUTO;
			uint32_t divisor = 0;
		};

		Attribute& add(const Attribute& element)
		{
			attributes.push_back(element);
			return attributes[attributes.size() - 1];
		}

	public:
		std::vector<Attribute> attributes;
	};
}