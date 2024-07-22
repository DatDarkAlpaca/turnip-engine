#pragma once
#include <string>
#include "Core/Config/ConfigSystem.hpp"

#include "Builders/InstanceBuilder.hpp"
#include "Objects/Surface.hpp"
#include "Builders/PhysicalDeviceBuilder.hpp"
#include "Objects/QueueFamily.hpp"
#include "Builders/LogicalDeviceBuilder.hpp"
#include "Objects/Queue.hpp"
#include "Builders/VMABuilder.hpp"
#include "Builders/SwapchainBuilder.hpp"

#include "Platform/Platform.hpp"

namespace tur::vulkan
{
	class DefaultVulkanInitializer
	{
	public:
		DefaultVulkanInitializer(const ConfigSystem& configSystem, platform::Window& window)
			: m_ConfigSystem(configSystem)
			, r_Window(window)
		{

		}

	public:
		void Initialize()
		{
			// Config:
			auto& applicationSpecification = m_ConfigSystem.GetApplicationSpecification();
			auto& windowProperties = m_ConfigSystem.GetWindowProperties();
			auto& graphicsSpecification = m_ConfigSystem.GetGraphicsSpecification();
			auto& vulkanArguments = m_ConfigSystem.GetVulkanArguments();

			// Vulkan Instance:
			InstanceObject instanceObject;
			{
				instanceBuilder
					.SetAppName(applicationSpecification.applicationName)
					.SetApplicationVersion(
						applicationSpecification.versionMajor,
						applicationSpecification.versionMinor,
						applicationSpecification.versionPatch,
						applicationSpecification.versionVariant)
					.SetEngineName(TUR_ENGINE_NAME)
					.SetEngineVersion(1, 0, 0, 0)
					.SetAPIVersion(
						graphicsSpecification.major,
						graphicsSpecification.minor,
						graphicsSpecification.patch,
						graphicsSpecification.variant
					);

				// Debug:
				{
					if (vulkanArguments.addDebugExtensions)
						instanceBuilder.AddDebugUtilsExtension();

					if (vulkanArguments.addValidationLayer)
						instanceBuilder.AddValidationLayer();

					instanceBuilder.UseDebugMessenger(vulkanArguments.useDebugMessenger);
				}

				instanceBuilder.ToggleKHRSurfaceFlag(vulkanArguments.enablePresentation);
				instanceBuilder.ToggleWindowingSurfaceFlag(vulkanArguments.enablePresentation);

				instanceBuilder.AddLayers(vulkanArguments.layers);
				instanceBuilder.AddExtensions(vulkanArguments.extensions);

				auto instanceBuilderResult = instanceBuilder.Build();
				if (!instanceBuilderResult.has_value())
					TUR_LOG_CRITICAL("[Vulkan Initializer]: Failed to initialize a valid Vulkan instance");

				instanceObject = instanceBuilderResult.value();
			}

			// Platform Surface:
			SurfaceObject surfaceObject = {};
			if (vulkanArguments.enablePresentation)
				surfaceObject.surface = platform::vulkan::GetVulkanSurface(instanceObject.instance, &r_Window);

			// Physical Device:
			PhysicalDeviceObject physicalDeviceObject;
			{
				physicalDeviceSelector.SetInstanceObject(instanceObject)
					.SetConfigSystem(m_ConfigSystem);

				if (vulkanArguments.enablePresentation)
				{
					physicalDeviceSelector.SetEnablePresentation(true)
						.SetSurfaceObject(surfaceObject)
						.AddRequiredExtension(vulkan::SwapchainExtensionName);

					physicalDeviceObject = physicalDeviceSelector.Select();
				}
			}

			// Queue Family Query:
			std::vector<QueueFamilyInformation> queueFamilyInfo;
			{
				queueFamilyInfo = GetQueueFamilyInformation(physicalDeviceObject.physicalDevice, surfaceObject.surface);
			}

			// Queue Selection:
			uint32_t presentQueueIndex = InvalidQueueIndex, graphicsQueueIndex = InvalidQueueIndex;
			{
				for (const auto& queueFamily : queueFamilyInfo)
				{
					if (GetQueueFamilySupports(queueFamily, QueueOperation::PRESENT))
						presentQueueIndex = queueFamily.familyIndex;

					if (GetQueueFamilySupports(queueFamily, QueueOperation::GRAPHICS))
						graphicsQueueIndex = queueFamily.familyIndex;
				}

				if (presentQueueIndex == vulkan::InvalidQueueIndex)
					TUR_LOG_ERROR("Failed to assign a present queue index");

				if (graphicsQueueIndex == vulkan::InvalidQueueIndex)
					TUR_LOG_ERROR("Failed to assign a graphics queue index");
			}

			// Logical Device:
			LogicalDeviceObject logicalDeviceObject;
			{
				logicalDeviceBuilder
					.SetInstanceObject(instanceObject)
					.SetPhysicalDeviceObject(physicalDeviceObject);

				// Queues:
				{
					logicalDeviceBuilder.PrepareQueueInfo(presentQueueIndex, 1.0f);
					if (graphicsQueueIndex != presentQueueIndex)
						logicalDeviceBuilder.PrepareQueueInfo(graphicsQueueIndex, 1.0f);
				}

				auto logicalDeviceResult = logicalDeviceBuilder.Create();
				if (!logicalDeviceResult.has_value())
					TUR_LOG_CRITICAL("Vulkan Initializer: Failed to create logical device");

				// Logical Device:
				logicalDeviceObject = logicalDeviceResult.value();
			}

			// Queues:
			QueueCluster queueCluster;
			{
				// Queues:
				auto graphicsQueue = logicalDeviceObject.device.getQueue(graphicsQueueIndex, 0);
				auto presentQueue = logicalDeviceObject.device.getQueue(presentQueueIndex, 0);

				queueCluster.Add(graphicsQueue, QueueOperation::GRAPHICS, graphicsQueueIndex);
				queueCluster.Add(presentQueue, QueueOperation::PRESENT, presentQueueIndex);
			}

			// VMA Allocator:
			VmaAllocator allocator;
			{
				vmaBuilder.SetArguments(
					instanceObject.instance, 
					physicalDeviceObject.physicalDevice, 
					logicalDeviceObject.device
				);

				allocator = vmaBuilder.Build();
			}

			// Swapchain:
			SwapchainObject swapchainObject;
			{
				auto& physicalDevice = physicalDeviceObject.physicalDevice;
				auto& logicalDevice = logicalDeviceObject.device;
				auto& surface = surfaceObject.surface;
				auto& queues = queueCluster.queues;

				auto& capabilities = QuerySurfaceCapabilities(physicalDevice, surface);
				auto& surfaceFormats = QuerySurfaceFormats(physicalDevice, surface);
				auto& presentModes = QuerySurfacePresentModes(physicalDevice, surface);

#ifdef TUR_DEBUG
				DisplaySurfaceCapabilities(capabilities);
				DisplaySurfaceFormats(surfaceFormats);
				DisplayPresentModes(presentModes);
#endif

				swapchainBuilder.SetArguments(surface, physicalDevice, logicalDevice, queueCluster)
								.Prepare();

				auto swapchainResult = swapchainBuilder.Create();
				if (swapchainResult.has_value())
					swapchainObject = swapchainResult.value();

				// Image & Image Views:
				// TODO: this;
				// swapchainFrameBuilder.Build(logicalDevice, device->swapchain);
			}
		}

	private:
		ConfigSystem m_ConfigSystem;
		platform::Window& r_Window;

	private:
		InstanceBuilder instanceBuilder;
		PhysicalDeviceSelector physicalDeviceSelector;
		LogicalDeviceBuilder logicalDeviceBuilder;
		VMABuilder vmaBuilder;
		SwapchainBuilder swapchainBuilder;
	};
}