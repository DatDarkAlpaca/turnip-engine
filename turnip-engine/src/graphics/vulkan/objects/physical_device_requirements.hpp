#pragma once
#include <string>
#include <vector>

namespace tur::vulkan
{
	struct PhysicalDeviceRequirements
	{
		// Mandatory:
		std::vector<std::string> extensions;

		// Optional:
		bool preferDiscrete = true;
	};
}