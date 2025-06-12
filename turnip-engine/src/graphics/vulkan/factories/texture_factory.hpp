#pragma once
#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>
#include "core/assets/texture_asset.hpp"
#include "graphics/vulkan/objects/texture.hpp"

namespace tur::vulkan
{
	class GraphicsDeviceVulkan;

	Texture build_texture(GraphicsDeviceVulkan* device, const TextureDescriptor& descriptor, const TextureAsset& asset);
}