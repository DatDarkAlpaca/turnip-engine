#pragma once
#include <string>
#include "Core/Config/ConfigSystem.hpp"

#include "Builders/InstanceBuilder.hpp"
#include "Objects/Surface.hpp"
#include "Builders/PhysicalDeviceBuilder.hpp"
#include "Objects/QueueFamily.hpp"

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
			SurfaceObject surfaceObject;
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
		}

	private:
		ConfigSystem m_ConfigSystem;
		platform::Window& r_Window;

	private:
		InstanceBuilder instanceBuilder;
		PhysicalDeviceSelector physicalDeviceSelector;
	};
}