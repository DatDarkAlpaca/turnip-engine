#pragma once
#include <vulkan/vulkan.hpp>

namespace tur::vulkan
{
	struct Pipeline
	{
	public:
		operator vk::Pipeline() { return pipeline; }

	public:
		vk::Pipeline pipeline;
		vk::PipelineLayout layout;
	};
}