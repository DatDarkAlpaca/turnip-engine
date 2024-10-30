#pragma once
#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <limits>

#include "Core/Config/ConfigData.hpp"
#include "Graphics/Vulkan/Objects/Instance.hpp"
#include "Graphics/Vulkan/Objects/Surface.hpp"
#include "Graphics/Vulkan/Objects/PhysicalDevice.hpp"

#include "Common.hpp"

namespace tur::vulkan
{
	class PhysicalDeviceSelector
	{
	public:
		using AvailableDevices = std::vector<vk::PhysicalDevice>;

	public:
		PhysicalDeviceSelector() = default;

	public:
		PhysicalDeviceObject SelectUsing(const std::function<vk::PhysicalDevice(const PhysicalDeviceSelector&, const AvailableDevices&)>&) const;

		PhysicalDeviceObject Select() const;

	public:
		PhysicalDeviceSelector& AddRequiredExtensions(const std::vector<const char*>& extensions);

		PhysicalDeviceSelector& AddRequiredExtension(const char* extensionName);

	public:
		PhysicalDeviceSelector& SetConfigData(const ConfigData& config);

		PhysicalDeviceSelector& SetInstanceObject(const InstanceObject& instanceObject);

		PhysicalDeviceSelector& SetSurfaceObject(const SurfaceObject& surfaceObject);

	private:
		std::optional<vk::PhysicalDevice> ChoosePhysicalDevice() const;

		bool DoesDeviceSupportRequirements(const vk::PhysicalDevice& device) const;

	public:
		inline const InstanceObject& GetInstanceObject() const { return m_InstanceObject; }
		inline vk::Instance GetInstance() const { return m_InstanceObject.instance; }
		inline vk::SurfaceKHR GetSurface() const { return m_SurfaceObject.surface; }
		inline const ConfigData& GetConfigData() const { return m_ConfigData; }

		const std::vector<const char*>& GetRequestedExtensions() const { return m_RequestedExtensions; }

	private:
		ConfigData m_ConfigData;
		InstanceObject m_InstanceObject;
		SurfaceObject m_SurfaceObject;

		std::vector<const char*> m_RequestedExtensions = {};

		bool m_InstanceSet = false;
		bool m_SurfaceSet = false;
		bool m_ConfigDataSet = false;
	};
}