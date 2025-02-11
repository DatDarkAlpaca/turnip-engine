#pragma once
#include <vector>
#include <vulkan/vulkan.hpp>

#include "common.hpp"

namespace tur::vulkan
{
	inline bool check_extension_support(const std::vector<vk::ExtensionProperties>& availableExtensions, const char* extensionName)
	{
		if (!extensionName)
			return false;

		for (const auto& extensionProperties : availableExtensions)
		{
			if (strcmp(extensionName, extensionProperties.extensionName) == 0)
				return true;
		}

		return false;
	}

	inline bool check_layer_supported(const std::vector<vk::LayerProperties>& availableLayers, const char* layerName)
	{
		if (!layerName)
			return false;

		for (const auto& layerProperties : availableLayers)
		{
			if (strcmp(layerName, layerProperties.layerName) == 0)
				return true;
		}

		return false;
	}

	struct ValidationResults
	{
		bool success = false;
		std::vector<const char*> unsupported;
	};

	inline ValidationResults validate_extensions(const std::vector<vk::ExtensionProperties>& supportedExtensions, const std::vector<const char*>& extensions)
	{
		ValidationResults results;
		results.success = true;

		for (const char* extension : extensions)
		{
			if (!check_extension_support(supportedExtensions, extension))
			{
				results.unsupported.push_back(extension);
				results.success = false;
			}
		}

		return results;
	}

	inline ValidationResults validate_layers(const std::vector<vk::LayerProperties>& supportedLayers, const std::vector<const char*>& layers)
	{

		ValidationResults results;
		results.success = true;

		for (const char* layer : layers)
		{
			if (!check_layer_supported(supportedLayers, layer))
			{
				results.unsupported.push_back(layer);
				results.success = false;
			}
		}

		return results;
	}
}