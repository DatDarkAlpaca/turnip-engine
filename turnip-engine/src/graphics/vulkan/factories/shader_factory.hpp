#pragma once
#include <vulkan/vulkan.hpp>
#include "graphics/objects/shader.hpp"
#include "utils/file.hpp"

namespace tur::vulkan
{
	vk::ShaderModule build_shader_module(vk::Device device, const ShaderDescriptor& descriptor);
}