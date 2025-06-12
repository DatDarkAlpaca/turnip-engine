#pragma once
#include <vector>
#include "graphics/objects/buffer.hpp"
#include "graphics/objects/texture.hpp"

namespace tur::gl
{
	struct DescriptorSet
	{
		std::vector<buffer_handle> boundBuffers;
		std::vector<texture_handle> boundTextures;
	};

	struct Descriptor
	{
		std::vector<descriptor_set_handle> sets;
	};
}