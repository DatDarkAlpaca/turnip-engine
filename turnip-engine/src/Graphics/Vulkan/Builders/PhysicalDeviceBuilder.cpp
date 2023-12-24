#include "pch.h"
#include "Graphics/Vulkan/Constants.h"
#include "PhysicalDeviceBuilder.h"

namespace tur::vulkan
{
	vk::PhysicalDevice DefaultPhysicalDeviceSelector(const PhysicalDeviceSelector& deviceSelector, const std::vector<vk::PhysicalDevice>& physicalDevices)
	{
		const auto& instanceOutput = deviceSelector.GetInstanceOutput();
		const auto& surface = deviceSelector.GetSurface();
		const auto& requestedExtensions = deviceSelector.GetRequestedExtensions();

		using score = uint32_t;

		std::vector<score> scoreList(physicalDevices.size());
		std::fill(scoreList.begin(), scoreList.end(), -1);

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

			// Required Extensions:
			bool supported;
			for (const auto& extension : requestedExtensions)
			{
				supported = false;
				for (const auto& properties : extensionProperties)
				{
					if (properties.extensionName == extension)
					{
						supported = true;
						scoreList[i] += 1;
						break;
					}
				}
			}

			if (!supported)
				continue;
			else
				scoreList[i] += 1;

			// Required Queue Support:
			if (instanceOutput.enablePresentation)
			{
				// Queues:
				auto queueFamiliesSupported = GetQueueFamilyInformation(device, surface);
				bool foundSupported = false;
				for (const auto& queueFamily : queueFamiliesSupported)
				{
					if (GetQueueFamilySupports(queueFamily, QueueOperation::GRAPHICS & QueueOperation::PRESENT))
					{
						scoreList[i] += 1;
						foundSupported = true;
						break;
					}
				}

				if (!foundSupported)
					continue;
			}

			// Favors discrete GPUs:
			if (deviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
				scoreList[i] += 2;
		}

		// Favors most memory:
		scoreList[maxMemoryElementIndex] += 1;

		size_t winnerIndex = std::distance(scoreList.begin(), std::max_element(scoreList.begin(), scoreList.end()));

		// probably doesn't work:
		if (scoreList[winnerIndex] == -1)
			TUR_LOG_CRITICAL("None of the available physical devices support the features requested.");

		return physicalDevices[winnerIndex];
	}

	PhysicalDevice PhysicalDeviceSelector::SelectUsing(const std::function<vk::PhysicalDevice(const PhysicalDeviceSelector&, const AvailableDevices&)>& selectorFunction) const
	{
		if (!m_InstanceSet)
			TUR_LOG_CRITICAL("Instance not set. Use SetInstance() before selecting a physical device");

		if (!m_SurfaceSet && m_InstanceOutput.enablePresentation)
			TUR_LOG_CRITICAL("Surface not set and instance requires presentation. Use SetSurface() before selecting a physical device");

		std::vector<vk::PhysicalDevice> availableDevices = GetInstance().enumeratePhysicalDevices();
		auto physicalDevice = selectorFunction(*this, availableDevices);

		PhysicalDevice output;
		output.physicalDevice = physicalDevice;
		output.queueFamilyInformation = GetQueueFamilyInformation(physicalDevice, m_Surface);

		return output;
	}

	PhysicalDevice PhysicalDeviceSelector::Select() const
	{
		auto deviceResult = ChoosePhysicalDevice();
		if (!deviceResult.has_value())
			TUR_LOG_CRITICAL("Selection parameters failed to return a valid physical device");

		vk::PhysicalDevice physicalDevice = deviceResult.value();

		PhysicalDevice output;
		output.physicalDevice = physicalDevice;
		output.queueFamilyInformation = GetQueueFamilyInformation(physicalDevice, m_Surface);

		return output;
	}

	std::optional<vk::PhysicalDevice> PhysicalDeviceSelector::ChoosePhysicalDevice() const
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

	bool PhysicalDeviceSelector::DoesDeviceSupportRequirements(const vk::PhysicalDevice& device) const
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
			for (const auto& queueFamily : GetQueueFamilyInformation(device, m_Surface))
			{
				if (GetQueueFamilySupports(queueFamily, QueueOperation::GRAPHICS | QueueOperation::PRESENT))
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

	PhysicalDeviceSelector& PhysicalDeviceSelector::SetInstance(const Instance& instanceOutput)
	{
		m_InstanceOutput = instanceOutput;
		m_InstanceSet = true;

		if (m_InstanceOutput.enablePresentation)
			m_RequestedExtensions.push_back(vulkan::SwapchainExtensionName);

		return *this;
	}

	PhysicalDeviceSelector& PhysicalDeviceSelector::SetSurface(const vk::SurfaceKHR& surface)
	{
		m_Surface = surface;
		m_SurfaceSet = true;
		return *this;
	}

	PhysicalDeviceSelector& PhysicalDeviceSelector::AddRequiredExtensions(const std::vector<const char*>& extensions)
	{
		for (const auto& extension : extensions)
			m_RequestedExtensions.push_back(extension);

		return *this;
	}

	PhysicalDeviceSelector& PhysicalDeviceSelector::AddRequiredExtension(const char* extensionName)
	{
		m_RequestedExtensions.push_back(extensionName);
		return *this;
	}
}