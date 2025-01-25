#include "pch.hpp"
#include "logical_device_builder.hpp"

#include "graphics/vulkan/vulkan_helpers.hpp"
#include "graphics/vulkan/vulkan_constants.hpp"
#include "graphics/vulkan/objects/queue_family.hpp"

namespace tur::vulkan
{
	void initialize_logical_device(VulkanState& state, const VulkanConfiguration& vulkanConfig)
	{
		using queue_family_index = handle_type;

		const auto& requiredQueueOperations = vulkanConfig.logicalDeviceRequirements.requiredQueueOperations;
		const auto& queueFamilies = get_queue_families(state.physicalDevice, state.surface);

		// Queue Selection:
		std::unordered_map<QueueUsage, bool> requiredUsageSatisfied;
		std::set<queue_family_index> familyIndices;
		for (const auto& operation : requiredQueueOperations)
			requiredUsageSatisfied[operation] = false;

		// Pre-screening:
		std::unordered_map<queue_family_index, u32> queueFamilyResponsibility;
		for (const auto& [queueFamilyIndex, supportedUsage] : queueFamilies)
		{
			for (const auto& operation : requiredQueueOperations) 
			{
				if (static_cast<u16>(supportedUsage) & static_cast<u16>(operation))
				{
					if (requiredUsageSatisfied[operation])
						continue;

					requiredUsageSatisfied[operation] = true;
					familyIndices.insert(queueFamilyIndex);

					queueFamilyResponsibility[queueFamilyIndex] |= static_cast<u32>(operation);
				}
			}
		}

		std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
		for (const auto& index : familyIndices)
		{
			float priority = 1.0f;
			vk::DeviceQueueCreateInfo createInfo{
				vk::DeviceQueueCreateFlags(),
				index,
				1,
				&priority
			};

			queueCreateInfos.push_back(createInfo);
		}

		// Extensions:
		std::vector<const char*> extensions;
		{
			for (const auto& extension : vulkanConfig.physicalDeviceRequirements.extensions)
				extensions.push_back(extension.c_str());

			if (state.requiresDrawing)
				extensions.push_back(SwapchainExtensionName);
		}
		
		// Features:
		vk::PhysicalDeviceVulkan13Features vulkan13Features;
		vk::PhysicalDeviceFeatures deviceFeatures = vk::PhysicalDeviceFeatures();
		{
			const auto& strExtensions = vulkanConfig.physicalDeviceRequirements.extensions;

			if (std::find(strExtensions.begin(), strExtensions.end(), VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME) != strExtensions.end())
				vulkan13Features.dynamicRendering = true;
			
			if (std::find(strExtensions.begin(), strExtensions.end(), VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME) != strExtensions.end())
				vulkan13Features.synchronization2 = true;
		}

		std::vector<const char*> layers;
		for (const auto& layer : vulkanConfig.instanceRequirements.layers)
			layers.push_back(layer.c_str());

		vk::DeviceCreateInfo deviceInfo = vk::DeviceCreateInfo(
			vk::DeviceCreateFlags(),
			static_cast<u32>(queueCreateInfos.size()),
			queueCreateInfos.data(),
			static_cast<u32>(layers.size()),
			layers.data(),
			static_cast<u32>(extensions.size()),
			extensions.data(),
			&deviceFeatures,
			&vulkan13Features
		);

		try
		{
			state.logicalDevice = state.physicalDevice.createDevice(deviceInfo);
		}
		catch (const std::exception& err)
		{
			TUR_LOG_ERROR("Failed to create logical device: {}", err.what());
		}

		// Queue Creation:
		for (const auto& [queueFamilyIndex, operation] : queueFamilyResponsibility)
		{
			vk::Queue queue = state.logicalDevice.getQueue(queueFamilyIndex, 0);
			state.queueList.add_queue(queue, queueFamilyIndex, static_cast<QueueUsage>(operation));
		}		
	}
}