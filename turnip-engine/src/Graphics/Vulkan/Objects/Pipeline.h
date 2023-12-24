#pragma once
#include <vulkan/vulkan.hpp>
#include "Pipeline.h"

namespace tur::vulkan
{
	struct PipelineVulkan : public IPipeline
	{
		vk::Pipeline pipeline;
		vk::PipelineLayout layout;
	};
}