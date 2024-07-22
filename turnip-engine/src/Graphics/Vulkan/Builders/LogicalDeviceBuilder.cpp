#include "pch.hpp"
#include "Common.hpp"
#include "LogicalDeviceBuilder.hpp"
#include "Graphics/Vulkan/Constants.hpp"

namespace tur::vulkan
{
	std::optional<LogicalDeviceObject> LogicalDeviceBuilder::Create() const
	{
		if (!m_InstanceSet)
			TUR_LOG_CRITICAL("[Vulkan Logical Device]: Instance not set. Use SetInstance() before creating a logical device");

		if (!m_DeviceSet)
			TUR_LOG_CRITICAL("[Vulkan Logical Device]: Physical device not set. Use SetPhysicalDevice() before creating a logical device");
		
		if (!m_ConfigSystemSet)
			TUR_LOG_CRITICAL("[Vulkan Logical Device]: ConfigSystem not set. Use SetConfigSystem() before creating a logical device");

		vk::PhysicalDeviceFeatures deviceFeatures = vk::PhysicalDeviceFeatures();

		vk::DeviceCreateInfo deviceInfo = vk::DeviceCreateInfo(
			vk::DeviceCreateFlags(),
			(uint32_t)m_QueueInfoList.size(), m_QueueInfoList.data(),
			0, nullptr,
			(uint32_t)m_Extensions.size(), m_Extensions.data(),
			&deviceFeatures
		);

		LogicalDeviceObject logicalDeviceObject;

		try
		{
			logicalDeviceObject.device = m_DeviceObject.physicalDevice.createDevice(deviceInfo);
			return logicalDeviceObject;
		}
		catch (const vk::SystemError& err)
		{
			TUR_LOG_ERROR("Failed to create logical device: {}", err.what());		
		}

		return std::nullopt;
	}

	LogicalDeviceBuilder& LogicalDeviceBuilder::SetInstanceObject(const InstanceObject& instanceObject)
	{
		m_InstanceObject = instanceObject;
		m_InstanceSet = true;

		if (m_ConfigSystem.GetVulkanArguments().enablePresentation)
			AddRequiredExtension(vulkan::SwapchainExtensionName);

		return *this;
	}

	LogicalDeviceBuilder& LogicalDeviceBuilder::SetPhysicalDeviceObject(const PhysicalDeviceObject& deviceObject)
	{
		m_DeviceObject = deviceObject;
		m_DeviceSet = true;
		return *this;
	}

	LogicalDeviceBuilder& LogicalDeviceBuilder::SetConfigSystem(const ConfigSystem& configSystem)
	{
		m_ConfigSystem = configSystem;
		m_ConfigSystemSet = true;
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