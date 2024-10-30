#pragma once
#include <nlohmann/json.hpp>
#include <vector>

namespace tur::vulkan
{
	struct VulkanArguments
	{
		// Instance:
		std::vector<std::string> layers = {}, extensions = {};
		bool enablePresentation = true;
		bool addValidationLayer = false, addDebugExtensions = false, useDebugMessenger = false;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(VulkanArguments,
			layers, extensions, enablePresentation, addValidationLayer, useDebugMessenger
		);
	};
}