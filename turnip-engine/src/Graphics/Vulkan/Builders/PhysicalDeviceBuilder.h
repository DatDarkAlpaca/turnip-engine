#pragma once
#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <limits>

#include "Common.h"
#include <vulkan/vulkan.hpp>

#include "Graphics/Vulkan/Objects/Queue.h"
#include "Graphics/Vulkan/Objects/QueueFamily.h"
#include "Graphics/Vulkan/Objects/Instance.h"
#include "Graphics/Vulkan/Objects/PhysicalDevice.h"

namespace tur::vulkan
{
	class PhysicalDeviceSelector
	{
	public:
		using AvailableDevices = std::vector<vk::PhysicalDevice>;

	public:
		PhysicalDevice SelectUsing(const std::function<vk::PhysicalDevice(const PhysicalDeviceSelector&, const AvailableDevices&)>&) const;

		PhysicalDevice Select() const;

	private:
		std::optional<vk::PhysicalDevice> ChoosePhysicalDevice() const;

		bool DoesDeviceSupportRequirements(const vk::PhysicalDevice& device) const;
		
	public:
		PhysicalDeviceSelector& SetInstance(const Instance& instanceOutput);

		PhysicalDeviceSelector& SetSurface(const vk::SurfaceKHR& surface);

	public:
		PhysicalDeviceSelector& AddRequiredExtensions(const std::vector<const char*>& extensions);

		PhysicalDeviceSelector& AddRequiredExtension(const char* extensionName);

	public:
		inline const Instance& GetInstanceOutput() const { return m_InstanceOutput; }
		inline vk::Instance GetInstance() const { return m_InstanceOutput.instanceHandle; }
		inline vk::SurfaceKHR GetSurface() const { return m_Surface; }

		const std::vector<const char*>& GetRequestedExtensions() const { return m_RequestedExtensions; }

	private:
		Instance m_InstanceOutput;
		vk::SurfaceKHR m_Surface;

		std::vector<const char*> m_RequestedExtensions;
		
		bool m_InstanceSet = false;
		bool m_SurfaceSet = false;
	};

	vk::PhysicalDevice DefaultPhysicalDeviceSelector(const PhysicalDeviceSelector& deviceSelector, const std::vector<vk::PhysicalDevice>& physicalDevices);
}