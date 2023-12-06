#pragma once
#include <functional>
#include <iostream>
#include <string>
#include <limits>
#include <vulkan/vulkan.hpp>

#include "Instance.h"

namespace tur::vulkan
{
	enum class PhysicalDeviceType
	{
		NONE				= 0,
		CPU					= VK_PHYSICAL_DEVICE_TYPE_CPU,
		IntegratedGpu		= VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU,
		DiscreteGpu			= VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU,
		VirtualGpu			= VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU,
		Other				= VK_PHYSICAL_DEVICE_TYPE_OTHER
	};

	class PhysicalDeviceSelector
	{
	public:
		using AvailableDevices = std::vector<vk::PhysicalDevice>;

	public:
		vk::PhysicalDevice Select(const std::function<vk::PhysicalDevice(const PhysicalDeviceSelector&, const AvailableDevices&)>& selectorFunction)
		{
			std::vector<vk::PhysicalDevice> availableDevices = GetInstance().enumeratePhysicalDevices();
			return selectorFunction(*this, availableDevices);
		}

	public:
		void SetInstance(const InstanceOutput& instanceOutput)
		{
			m_InstanceOutput = instanceOutput;
			m_InstanceSet = true;

			m_InstanceOutput.enablePresentation;
		}

	public:
		const InstanceOutput& GetInstanceOutput() const { return m_InstanceOutput; }

		vk::Instance GetInstance() const { return m_InstanceOutput.instanceHandle; }

	private:
		InstanceOutput m_InstanceOutput;
		bool m_InstanceSet = false;
	};

	inline vk::PhysicalDevice DefaultPhysicalDeviceSelector(const PhysicalDeviceSelector& deviceSelector, const std::vector<vk::PhysicalDevice>& physicalDevices)
	{
		const auto& instanceOutput = deviceSelector.GetInstanceOutput();
		using score = uint32_t;

		std::vector<score> scoreList(physicalDevices.size());

		// Get device with most memory:
		size_t maxMemoryElementIndex = 0;
		auto maxMemoryElement = std::max_element(physicalDevices.begin(), physicalDevices.end(), [](vk::PhysicalDevice lhs, vk::PhysicalDevice rhs) 
		{
			auto memoryLHS = lhs.getMemoryProperties();
			auto memoryRHS = rhs.getMemoryProperties();

			size_t heapSizeLHS = 0, heapSizeRHS = 0;
			for (size_t i = 0; i < memoryLHS.memoryHeapCount; ++i)
			{
				if (memoryLHS.memoryHeaps[i].flags & vk::MemoryHeapFlagBits::eDeviceLocal)
					heapSizeLHS = memoryLHS.memoryHeaps[i].size;
			}

			for (size_t i = 0; i < memoryRHS.memoryHeapCount; ++i)
			{
				if (memoryRHS.memoryHeaps[i].flags & vk::MemoryHeapFlagBits::eDeviceLocal)
					heapSizeRHS = memoryRHS.memoryHeaps[i].size;
			}

			return (heapSizeLHS > heapSizeRHS);
		});
		maxMemoryElementIndex = std::distance(physicalDevices.begin(), maxMemoryElement);

		for (size_t i = 0; i < physicalDevices.size(); ++i)
		{
			auto& device = physicalDevices[i];

			auto extensionProperties = device.enumerateDeviceExtensionProperties();
			auto deviceProperties = device.getProperties();
			auto memoryProperties = device.getMemoryProperties();

			// Requires swapchain extension:
			if (instanceOutput.enablePresentation)
			{
				bool found = false;
				for (const auto& extension : extensionProperties)
				{
					if (extension.extensionName == vulkan::SwapchainExtensionName)
					{
						found = true;
						break;
					}
				}

				if (!found)
					continue;
			}

			// Favors discrete GPUs:
			if(deviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
				scoreList[i] += 2;
		}
		
		// Favors most memory:
		scoreList[maxMemoryElementIndex] += 1;

		size_t winnerIndex = std::distance(scoreList.begin(), std::max_element(scoreList.begin(), scoreList.end()));
		return physicalDevices[winnerIndex];
	}
}