#pragma once
#include <functional>
#include <iostream>
#include <string>
#include <limits>
#include <vulkan/vulkan.hpp>

#include "Instance.h"
#include "Queues.h"

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

	struct PhysicalDeviceOutput
	{
		vk::PhysicalDevice device;
		std::vector<QueueFamilyInformation> queueInformation;
		std::vector<const char*> requestedExtensions;
	};

	class PhysicalDeviceSelector
	{
	public:
		using AvailableDevices = std::vector<vk::PhysicalDevice>;

	public:
		PhysicalDeviceOutput SelectUsing(const std::function<vk::PhysicalDevice(const PhysicalDeviceSelector&, const AvailableDevices&)>& selectorFunction) const
		{
			if (!m_InstanceSet)
				TUR_LOG_CRITICAL("Instance not set. Use SetInstance() before selecting a physical device");

			if (!m_SurfaceSet && m_InstanceOutput.enablePresentation)
				TUR_LOG_CRITICAL("Surface not set and instance requires presentation. Use SetSurface() before selecting a physical device");

			std::vector<vk::PhysicalDevice> availableDevices = GetInstance().enumeratePhysicalDevices();
			auto device = selectorFunction(*this, availableDevices);

			return { device, GetQueueInformation(device, m_Surface) };
		}

		PhysicalDeviceOutput Select() const
		{
			auto deviceResult = ChoosePhysicalDevice();
			if (!deviceResult.has_value())
				TUR_LOG_CRITICAL("Selection parameters failed to return a valid physical device");
			
			vk::PhysicalDevice device = deviceResult.value();
			
			PhysicalDeviceOutput output;
			output.device = device;
			output.queueInformation = GetQueueInformation(device, m_Surface);
			output.requestedExtensions = m_RequestedExtensions;

			return output;
		}

	private:
		std::optional<vk::PhysicalDevice> ChoosePhysicalDevice() const
		{
			std::vector<vk::PhysicalDevice> availableDevices = GetInstance().enumeratePhysicalDevices();

			for (const auto& device : availableDevices)
			{
				if (!DoesDeviceSupportRequirements(device))
					continue;

				return device;
			}

			return std::nullopt;
		}

		bool DoesDeviceSupportRequirements(const vk::PhysicalDevice& device) const
		{
			const auto& instanceOutput = GetInstanceOutput();
			const auto& surface = GetSurface();
			const auto& availableExtensions = device.enumerateDeviceExtensionProperties();

			bool requiresPresent = m_InstanceOutput.enablePresentation;
			
			bool supportsExtensions = false;
			bool supportsPresent = false;

			// Extensions:
			{
				supportsExtensions = true;
				for (const auto& extension : GetRequestedExtensions())
				{
					bool found = false;
					for (const auto& properties : device.enumerateDeviceExtensionProperties())
					{
						if (strcmp(properties.extensionName, extension) == 0)
						{
							found = true;
							break;
						}
					}

					supportsExtensions = found;
				}
			}

			// Queues:
			if (requiresPresent)
			{
				for (const auto& queue : GetQueueInformation(device, m_Surface))
				{
					if (GetQueueSupports(queue, QueueOperation::GRAPHICS | QueueOperation::PRESENT))
					{
						supportsPresent = true;
						break;
					}
				}
			}
			
			if (!requiresPresent)
				return supportsExtensions;

			return supportsExtensions && supportsPresent;
		}

	public:
		PhysicalDeviceSelector& SetInstance(const InstanceOutput& instanceOutput)
		{
			m_InstanceOutput = instanceOutput;
			m_InstanceSet = true;

			if (m_InstanceOutput.enablePresentation)
				m_RequestedExtensions.push_back(vulkan::SwapchainExtensionName);

			return *this;
		}

		PhysicalDeviceSelector& SetSurface(const vk::SurfaceKHR& surface)
		{
			m_Surface = surface;
			m_SurfaceSet = true;
			return *this;
		}

	public:
		PhysicalDeviceSelector& AddRequiredExtensions(const std::vector<const char*>& extensions)
		{
			for(const auto& extension : extensions)
				m_RequestedExtensions.push_back(extension);

			return *this;
		}
		PhysicalDeviceSelector& AddRequiredExtension(const char* extensionName)
		{
			m_RequestedExtensions.push_back(extensionName);
			return *this;
		}

	public:
		const InstanceOutput& GetInstanceOutput() const { return m_InstanceOutput; }
		vk::Instance GetInstance() const { return m_InstanceOutput.instanceHandle; }
		vk::SurfaceKHR GetSurface() const { return m_Surface; }

		const std::vector<const char*>& GetRequestedExtensions() const { return m_RequestedExtensions; }

	private:
		InstanceOutput m_InstanceOutput;
		vk::SurfaceKHR m_Surface;

		std::vector<const char*> m_RequestedExtensions;
		
		bool m_InstanceSet = false;
		bool m_SurfaceSet = false;
	};

	vk::PhysicalDevice DefaultPhysicalDeviceSelector(const PhysicalDeviceSelector& deviceSelector, const std::vector<vk::PhysicalDevice>& physicalDevices);
}