#pragma once
#include <vector>

namespace tur::vulkan
{
	struct VulkanArguments
	{
		// Instance:
		std::vector<std::string> layers = {}, extensions = {};
		bool enablePresentation = true;
		bool addValidationLayer = false, addDebugExtensions = false, useDebugMessenger = false;
	};
}