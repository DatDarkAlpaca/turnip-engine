#pragma once
#include <glm/glm.hpp>
#include "Graphics/Type/Rect.hpp"

namespace tur
{
	enum class DescriptorSetType
	{
		UNIFORM_BUFFER,
		SAMPLER,
		STORAGE_BUFFER
	};

	struct DescriptorSetLayout
	{
	public:
		constexpr static inline uint32_t AUTO = std::numeric_limits<uint32_t>::max();

	public:
		struct DescriptorSet
		{
			uint32_t binding;
			DescriptorSetType descriptorSetType;
			uint32_t descriptorCount;
		};

		DescriptorSet& add(const DescriptorSet& descriptorSet)
		{
			sets.push_back(descriptorSet);
			return sets[sets.size() - 1];
		}

	public:
		std::vector<DescriptorSet> sets;
	};
}