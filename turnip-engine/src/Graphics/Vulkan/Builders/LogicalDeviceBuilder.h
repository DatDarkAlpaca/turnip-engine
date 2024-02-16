#pragma once
#include <optional>
#include <vulkan/vulkan.hpp>

#include "Graphics/Vulkan/Objects/Objects.h"

namespace tur::vulkan
{
	class LogicalDeviceBuilder
	{
	public:
		std::optional<LogicalDevice> Create() const;

	public:
		LogicalDeviceBuilder& SetInstance(const Instance& instance);

		LogicalDeviceBuilder& SetPhysicalDevice(const PhysicalDevice& device);

	public:
		LogicalDeviceBuilder& PrepareQueueInfo(uint32_t queueIndex, float priority = 1.0f);

	public:
		LogicalDeviceBuilder& AddRequiredExtensions(const std::vector<const char*>& extensions);

		LogicalDeviceBuilder& AddRequiredExtension(const char* extensionName);

	private:
		PhysicalDevice m_Device;
		Instance m_Instance;

		std::vector<const char*> m_Extensions;
		std::vector<vk::DeviceQueueCreateInfo> m_QueueInfoList;
	};
}