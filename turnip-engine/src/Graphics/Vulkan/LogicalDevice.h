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
		std::optional<vk::Device> Build() const
		{
			vk::PhysicalDeviceFeatures deviceFeatures = vk::PhysicalDeviceFeatures();

			vk::DeviceCreateInfo deviceInfo = vk::DeviceCreateInfo(
				vk::DeviceCreateFlags(),
				m_QueueInfoList.size(), m_QueueInfoList.data(),
				m_Layers.size(), m_Layers.data(),
				m_Extensions.size(), m_Extensions.data(),
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

			if (instance.enableValidation)
				AddRequiredLayer(vulkan::ValidationLayerName);

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

		LogicalDeviceBuilder& AddRequiredLayers(const std::vector<const char*>& layers)
		{
			for (const auto& layer : layers)
				m_Layers.push_back(layer);

			return *this;
		}
		LogicalDeviceBuilder& AddRequiredLayer(const char* layer)
		{
			m_Layers.push_back(layer);
			return *this;
		}

	private:
		PhysicalDeviceOutput m_DeviceOutput;
		InstanceOutput m_InstanceOutput;

		std::vector<const char*> m_Extensions, m_Layers;
		std::vector<vk::DeviceQueueCreateInfo> m_QueueInfoList;
	};
}