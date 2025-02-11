#pragma once
#include <vulkan/vulkan.hpp>
#include "graphics/objects/pipeline.hpp"

namespace tur::vulkan
{
	class GraphicsDeviceVulkan;

	vk::Pipeline create_graphics_pipeline(GraphicsDeviceVulkan& device, const PipelineDescriptor& descriptor);
}