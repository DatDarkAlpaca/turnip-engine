#pragma once
#include <vulkan/vulkan.hpp>

#include "PhysicalDevice.h"

namespace tur::vulkan
{
	class LogicalDeviceBuilder
	{
	public:
		LogicalDeviceBuilder() = default;

	public:
		std::optional<vk::Device> Create() const
		{
			vk::PhysicalDeviceFeatures deviceFeatures = vk::PhysicalDeviceFeatures();

			vk::DeviceCreateInfo deviceInfo = vk::DeviceCreateInfo(
				vk::DeviceCreateFlags(),
				(uint32_t)m_QueueInfoList.size(), m_QueueInfoList.data(),
				0, nullptr,
				(uint32_t)m_Extensions.size(), m_Extensions.data(),
				&deviceFeatures
			);

			try
			{
				vk::Device device = m_DeviceOutput.device.createDevice(deviceInfo);
				return device;
			}
			catch (vk::SystemError err)
			{
				TUR_LOG_ERROR("Device creation failed: {}", err.what());
				return std::nullopt;
			}
		}

	public:
		LogicalDeviceBuilder& SetInstanceOutput(const InstanceOutput& instance)
		{
			m_InstanceOutput = instance;

			if (instance.enablePresentation)
				AddRequiredExtension(vulkan::SwapchainExtensionName);

			return *this;
		}

		LogicalDeviceBuilder& SetPhysicalDeviceOutput(const PhysicalDeviceOutput& device)
		{
			m_DeviceOutput = device;
			return *this;
		}

	public:
		LogicalDeviceBuilder& AddQueueInfo(const vk::DeviceQueueCreateInfo& queueCreateInfo)
		{
			m_QueueInfoList.push_back(queueCreateInfo);
			return *this;
		}

	public:
		LogicalDeviceBuilder& AddRequiredExtensions(const std::vector<const char*>& extensions)
		{
			for (const auto& extension : extensions)
				m_Extensions.push_back(extension);

			return *this;
		}
		LogicalDeviceBuilder& AddRequiredExtension(const char* extensionName)
		{
			m_Extensions.push_back(extensionName);
			return *this;
		}

	private:
		PhysicalDeviceOutput m_DeviceOutput;
		InstanceOutput m_InstanceOutput;

		std::vector<const char*> m_Extensions;
		std::vector<vk::DeviceQueueCreateInfo> m_QueueInfoList;
	};
}