#pragma once
#include <vulkan/vulkan.hpp>
#include "Pipeline.h"

namespace tur::vulkan
{
	struct PipelineVulkan : public Pipeline
	{
		vk::Pipeline pipeline;
		vk::PipelineLayout layout;
		vk::RenderPass renderpass;
	};
}