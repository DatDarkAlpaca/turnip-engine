#pragma once
#include <vector>
#include <string>

namespace tur::vulkan
{
	struct InstanceRequirements
	{
		std::vector<std::string> layers, extensions;

		bool enableDrawing = true;
		bool enableValidationLayers = false;
	};
}