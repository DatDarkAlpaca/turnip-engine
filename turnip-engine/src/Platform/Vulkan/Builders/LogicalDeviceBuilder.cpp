#include "pch.h"
#include "Common.h"
#include "LogicalDeviceBuilder.h"

namespace tur::vulkan
{
	std::optional<LogicalDevice> LogicalDeviceBuilder::Create() const
	{
		vk::PhysicalDeviceFeatures deviceFeatures = vk::PhysicalDeviceFeatures();

		vk::DeviceCreateInfo deviceInfo = vk::DeviceCreateInfo(
			vk::DeviceCreateFlags(),
			(uint32_t)m_QueueInfoList.size(), m_QueueInfoList.data(),
			0, nullptr,
			(uint32_t)m_Extensions.size(), m_Extensions.data(),
			&deviceFeatures
		);

		LogicalDevice output;

		try
		{
			output.device = m_Device.physicalDevice.createDevice(deviceInfo);
			return output;
		}
		catch (vk::SystemError err)
		{
			TUR_LOG_ERROR("Failed to create logical device: {}", err.what());		
		}

		return std::nullopt;
	}

	LogicalDeviceBuilder& LogicalDeviceBuilder::SetInstance(const Instance& instance)
	{
		m_Instance = instance;

		if (instance.enablePresentation)
			AddRequiredExtension(vulkan::SwapchainExtensionName);

		return *this;
	}

	LogicalDeviceBuilder& LogicalDeviceBuilder::SetPhysicalDevice(const PhysicalDevice& device)
	{
		m_Device = device;
		return *this;
	}

	LogicalDeviceBuilder& LogicalDeviceBuilder::PrepareQueueInfo(uint32_t queueIndex, float priority)
	{
		vk::DeviceQueueCreateInfo createInfo { 
			vk::DeviceQueueCreateFlags(), queueIndex, 1, & priority 
		};

		m_QueueInfoList.push_back(createInfo);
		return *this;
	}

	LogicalDeviceBuilder& LogicalDeviceBuilder::AddRequiredExtensions(const std::vector<const char*>& extensions)
	{
		for (const auto& extension : extensions)
			m_Extensions.push_back(extension);

		return *this;
	}

	LogicalDeviceBuilder& LogicalDeviceBuilder::AddRequiredExtension(const char* extensionName)
	{
		m_Extensions.push_back(extensionName);
		return *this;
	}
}