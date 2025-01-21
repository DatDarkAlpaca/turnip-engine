#include "pch.hpp"
#include "physical_device_builder.hpp"
#include "graphics/vulkan/vulkan_helpers.hpp"
#include "graphics/vulkan/objects/queue_family.hpp"

namespace tur::vulkan
{
	void initialize_physical_device(VulkanState& state, const PhysicalDeviceRequirements& requirements)
	{
		auto physicalDevices = state.instance.enumeratePhysicalDevices();
		if (physicalDevices.empty())
			TUR_LOG_CRITICAL("No valid vulkan physical devices found.");

		std::vector<const char*> extensions;
		for (const auto& extension : requirements.extensions)
			extensions.push_back(extension.c_str());

		std::vector<vk::PhysicalDevice> suitableDevices;
		{
			for (const auto& device : physicalDevices)
			{
				auto deviceName = device.getProperties().deviceName.data();

				auto extensionSupport = validate_extensions(device.enumerateDeviceExtensionProperties(), extensions);
				if (!extensionSupport.success)
				{
					TUR_LOG_INFO("{} does not support the following extensions: ", deviceName);
					
					for (const auto& unsupported : extensionSupport.unsupported)
						TUR_LOG_INFO(" * {}", unsupported);
				}

				auto supportedQueueFeatures = get_queue_family_supported_features(device, state.surface);
				if (state.requiresDrawing)
				{
					if (!(static_cast<u16>(supportedQueueFeatures) & static_cast<u16>(QueueUsage::GRAPHICS)))
						continue;

					if (!(static_cast<u16>(supportedQueueFeatures) & static_cast<u16>(QueueUsage::PRESENT)))
						continue;
				}

				suitableDevices.push_back(device);
			}
		}

		if (suitableDevices.size() == 0)
		{
			TUR_LOG_CRITICAL("No suitable devices were found.");
		}
		
		vk::PhysicalDevice chosenDevice;
		{
			for (const auto& device : suitableDevices)
			{
				const auto& properties = device.getProperties();
				
				if (requirements.preferDiscrete && properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
					chosenDevice = device;
			}
		}

		// TODO: handle invalid device
		state.physicalDevice = chosenDevice;
	}
}