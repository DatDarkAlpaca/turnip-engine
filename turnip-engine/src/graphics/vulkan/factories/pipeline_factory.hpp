#pragma once
#include <vulkan/vulkan.hpp>
#include "graphics/vulkan/objects/pipeline.hpp"

namespace tur::vulkan
{
	class GraphicsDeviceVulkan;

	Pipeline build_graphics_pipeline(GraphicsDeviceVulkan& device, const PipelineDescriptor& descriptor);
}