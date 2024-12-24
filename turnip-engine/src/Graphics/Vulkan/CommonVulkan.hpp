#pragma once
#include <vulkan/vulkan.hpp>

#include "Graphics/CommonGraphics.hpp"
#include "Constants.hpp"

namespace tur::vulkan
{
	using vk_handle = handle_type;

	static inline constexpr uint32_t invalid_queue_index = std::numeric_limits<uint32_t>::max();
}