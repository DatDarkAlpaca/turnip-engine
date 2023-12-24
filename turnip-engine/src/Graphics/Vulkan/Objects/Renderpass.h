#pragma once
#include <vulkan/vulkan.hpp>
#include "Graphics/Renderpass.h"

namespace tur::vulkan
{
	class RenderpassVulkan : public IRenderpass
	{
	public:
		vk::RenderPass renderpass;
	};
}