#pragma once
#include "Common.h"
#include "RenderDeviceVK.h"
#include "Rendering/RenderInitializer.h"

#include "Builders/InstanceBuilder.h"
#include "Builders/PhysicalDeviceBuilder.h"
#include "Builders/LogicalDeviceBuilder.h"
#include "Builders/SwapchainBuilder.h"
#include "Builders/FrameBuilder.h"

namespace tur::vulkan
{
    class VulkanInitializer : public RenderInitializer
    {
    public:
        virtual ~VulkanInitializer() = default;
        
    protected:
        VulkanInstanceBuilder instanceBuilder;
        PhysicalDeviceSelector physicalDeviceSelector;
        LogicalDeviceBuilder logicalDeviceBuilder;
        SwapchainBuilder swapchainBuilder;
        SwapchainFrameBuilder swapchainFrameBuilder;
    };

    class DefaultVulkanInitializer final : public VulkanInitializer
    {
    public:
        DefaultVulkanInitializer()
        {
            instanceBuilder
                .SetAppName("") /* TODO: Set Application Name */
                .SetEngineName("TurnipEngine")
                .SetAPIVersion(1, 0, 0);
        }

    public:
        void Initialize(NON_OWNING RenderDevice* _device) override
        {
            using namespace vulkan;

            RenderDeviceVK* device = static_cast<RenderDeviceVK*>(_device);

            // Instance:
            Instance instanceOutput;
            {
                auto instanceOutputResult = instanceBuilder.Build();

                if (!instanceOutputResult.has_value())
                    TUR_LOG_CRITICAL("Vulkan Initializer: Failed to initialize instance");

                instanceOutput = instanceOutputResult.value();

                device->instance = instanceOutput.instanceHandle;
                device->debugMessenger = instanceOutput.debugMessenger;

                instanceBuilder.DisplayVulkanAPIVersion();
                TUR_LOG_DEBUG("Initialized Vulkan Instance");
                TUR_LOG_DEBUG("Initialized Vulkan DebugMessenger");
            }

            // Surface:
            {
                device->surface = platform::GetVulkanSurface(instanceOutput.instanceHandle, device->GetWindow());
            }

            // Physical Device:
            PhysicalDevice physicalDeviceOutput;
            {
                physicalDeviceSelector.SetInstance(instanceOutput)
                                      .SetSurface(device->surface);

                physicalDeviceOutput = physicalDeviceSelector.Select();
                device->physicalDevice = physicalDeviceOutput.physicalDevice;

                TUR_LOG_DEBUG("Selected GPU: {}", physicalDeviceOutput.physicalDevice.getProperties().deviceName.data());
            }

            // Queue Selection:
            uint32_t presentQueueIndex = InvalidQueueIndex, graphicsQueueIndex = InvalidQueueIndex;
            {
                for (const auto& queueFamily : physicalDeviceOutput.queueFamilyInformation)
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
            
            // Logical Device & Queue:
            {
                logicalDeviceBuilder.SetInstance(instanceOutput)
                                    .SetPhysicalDevice(physicalDeviceOutput);

                logicalDeviceBuilder.PrepareQueueInfo(presentQueueIndex, 1.0f);

                if (graphicsQueueIndex != presentQueueIndex)
                    logicalDeviceBuilder.PrepareQueueInfo(graphicsQueueIndex, 1.0f);

                auto logicalDeviceResult = logicalDeviceBuilder.Create();
                if (!logicalDeviceResult.has_value())
                    TUR_LOG_CRITICAL("Vulkan Initializer: Failed to create logical device");

                // Logical Device:
                device->logicalDevice = logicalDeviceResult.value().device;

                // Queues:
                auto graphicsQueue = device->logicalDevice.getQueue(graphicsQueueIndex, 0);
                auto presentQueue = device->logicalDevice.getQueue(presentQueueIndex, 0);

                device->queues.Add(graphicsQueue, QueueOperation::GRAPHICS, graphicsQueueIndex);
                device->queues.Add(presentQueue, QueueOperation::PRESENT, presentQueueIndex);
            
                TUR_LOG_DEBUG("Initialized Vulkan Logical Device");
                TUR_LOG_DEBUG("Using Graphics Queue Family: {}", graphicsQueueIndex);
                TUR_LOG_DEBUG("Using Present Queue Family: {}", presentQueueIndex);
            }

            // Swapchain:
            {
                auto& physicalDevice = device->physicalDevice;
                auto& logicalDevice = device->logicalDevice;
                auto& surface = device->surface;
                auto& queues = device->queues;

                auto& capabilities = QuerySurfaceCapabilities(physicalDevice, surface);
                auto& surfaceFormats = QuerySurfaceFormats(physicalDevice, surface);
                auto& presentModes = QuerySurfacePresentModes(physicalDevice, surface);

                #ifdef TUR_DEBUG
                    DisplaySurfaceCapabilities(capabilities);
                    DisplaySurfaceFormats(surfaceFormats);
                    DisplayPresentModes(presentModes);
                #endif
    
                swapchainBuilder.SetArguments(surface, physicalDevice, logicalDevice, queues)
                                .Prepare();

                auto swapchainResult = swapchainBuilder.Create();
                if (swapchainResult.has_value())
                    device->swapchain = swapchainResult.value();

                // Image & Image Views:
                swapchainFrameBuilder.Build(logicalDevice, device->swapchain);
            }

            // Framebuffer & Default Renderpass for Swapchain Frames:
            device->FinishSetup();
        }
    };
}