#pragma once
#include <vulkan/vulkan.hpp>

namespace tur::vulkan
{
	struct Pipeline
	{
		vk::Pipeline pipeline;
		vk::PipelineLayout layout;
	};
}