#pragma once
#include <optional>
#include <vulkan/vulkan.hpp>

#include "Core/Config/ConfigSystem.hpp"

#include "Graphics/Vulkan/Objects/Instance.hpp"
#include "Graphics/Vulkan/Objects/PhysicalDevice.hpp"
#include "Graphics/Vulkan/Objects/LogicalDevice.hpp"

namespace tur::vulkan
{
	class LogicalDeviceBuilder
	{
	public:
		std::optional<LogicalDeviceObject> Create() const;

	public:
		LogicalDeviceBuilder& SetInstanceObject(const InstanceObject& instanceObject);

		LogicalDeviceBuilder& SetPhysicalDeviceObject(const PhysicalDeviceObject& deviceObject);

		LogicalDeviceBuilder& SetConfigSystem(const ConfigSystem& configSystem);

	public:
		LogicalDeviceBuilder& PrepareQueueInfo(uint32_t queueIndex, float priority = 1.0f);

	public:
		LogicalDeviceBuilder& AddRequiredExtensions(const std::vector<const char*>& extensions);

		LogicalDeviceBuilder& AddRequiredExtension(const char* extensionName);

	private:
		PhysicalDeviceObject m_DeviceObject;
		InstanceObject m_InstanceObject;
		ConfigSystem m_ConfigSystem;

		std::vector<const char*> m_Extensions;
		std::vector<vk::DeviceQueueCreateInfo> m_QueueInfoList;

		bool m_InstanceSet = false;
		bool m_DeviceSet = false;
		bool m_ConfigSystemSet = false;
	};
}