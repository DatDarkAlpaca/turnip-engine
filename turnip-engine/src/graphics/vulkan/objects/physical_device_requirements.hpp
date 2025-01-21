#pragma once
#include <string>
#include <vector>

namespace tur::vulkan
{
	struct PhysicalDeviceRequirements
	{
		// Mandatory:
		std::vector<std::string> extensions = {
			"VK_KHR_dynamic_rendering",
			"VK_EXT_descriptor_buffer"
		};

		// Optional:
		bool preferDiscrete = true;
	};
}