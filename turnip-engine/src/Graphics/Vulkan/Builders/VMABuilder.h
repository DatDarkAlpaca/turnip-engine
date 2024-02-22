#pragma once
#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>

namespace tur::vulkan
{
	class VMABuilder
	{
	public:
		VmaAllocator Build()
		{
			VmaVulkanFunctions vulkanFunctions = { };
			vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
			vulkanFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;

			VmaAllocatorCreateInfo allocatorCreateInfo = {};
			allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
			allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_2;
			allocatorCreateInfo.instance = reinterpret_cast<VkInstance>(&m_Instance);
			allocatorCreateInfo.physicalDevice = reinterpret_cast<VkPhysicalDevice>(&m_PhysicalDevice);
			allocatorCreateInfo.device = reinterpret_cast<VkDevice>(&m_Device);
			allocatorCreateInfo.pVulkanFunctions = &vulkanFunctions;

			VmaAllocator allocator;
			vmaCreateAllocator(&allocatorCreateInfo, &allocator);

			return allocator;
		}

	public:
		VMABuilder& SetArguments(vk::Instance instance, vk::PhysicalDevice physicalDevice, vk::Device device;)
		{
			m_Instance = instance;
			m_PhysicalDevice = physicalDevice;
			m_Device = device;

			m_ArgumentsSet = true;
		}

	private:
		vk::Instance m_Instance;
		vk::PhysicalDevice m_PhysicalDevice;
		vk::Device m_Device;

	private:
		bool m_ArgumentsSet = false;
	};
}