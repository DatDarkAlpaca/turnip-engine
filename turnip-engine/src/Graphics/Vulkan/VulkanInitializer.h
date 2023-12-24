#pragma once
#include "Common.h"
#include "Graphics/Vulkan/BackendVulkan.h"

#include "Objects/Queue.h"
#include "Objects/QueueFamily.h"

#include "Builders/InstanceBuilder.h"
#include "Builders/PhysicalDeviceBuilder.h"
#include "Builders/LogicalDeviceBuilder.h"

#include "Builders/SwapchainBuilder.h"
#include "Builders/FrameBuilder.h"

namespace tur
{
    class VulkanInitializer
    {
    public:
        VulkanInitializer(IGraphicsBackend* backend)
            : backend(VULKAN_BACKEND(backend)) { }

        virtual ~VulkanInitializer() = default;

    protected:
        NON_OWNING BackendVulkan* backend = nullptr;

    protected:
        vulkan::VulkanInstanceBuilder instanceBuilder;
        vulkan::PhysicalDeviceSelector physicalDeviceSelector;
        vulkan::LogicalDeviceBuilder logicalDeviceBuilder;
        vulkan::SwapchainBuilder swapchainBuilder;
        vulkan::SwapchainFrameBuilder swapchainFrameBuilder;
    };

    class DefaultVulkanInitializer : public VulkanInitializer
    {
    public:
        DefaultVulkanInitializer(IGraphicsBackend* graphicsBackend)
            : VulkanInitializer(graphicsBackend)
        {
            instanceBuilder
                .SetAppName(backend->Properties().applicationName)
                .SetEngineName("TurnipEngine")
                .SetAPIVersion(1, 0, 0);

            Configure();
        }

    private:
        void Configure()
        {
            using namespace vulkan;

            // Instance:
            Instance instanceOutput;
            {
                auto instanceOutputResult = instanceBuilder.Build();

                if (!instanceOutputResult.has_value())
                    TUR_LOG_CRITICAL("Vulkan Initializer: Failed to initialize instance");

                instanceOutput = instanceOutputResult.value();

                backend->Instance() = instanceOutput.instanceHandle;
                backend->DebugMessenger() = instanceOutput.debugMessenger;

                instanceBuilder.DisplayVulkanAPIVersion();
                TUR_LOG_DEBUG("Initialized Vulkan Instance");
                TUR_LOG_DEBUG("Initialized Vulkan DebugMessenger");
            }

            // Surface:
            {
                backend->SurfaceKHR() = platform::GetVulkanSurface(instanceOutput.instanceHandle, backend->GetWindow());
            }

            // Physical Device:
            PhysicalDevice physicalDeviceOutput;
            {
                physicalDeviceSelector.SetInstance(instanceOutput)
                                      .SetSurface(backend->SurfaceKHR());

                physicalDeviceOutput = physicalDeviceSelector.Select();
                backend->PhysicalDevice() = physicalDeviceOutput.physicalDevice;

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
                backend->Device() = logicalDeviceResult.value().device;

                // Queues:
                auto graphicsQueue = backend->Device().getQueue(graphicsQueueIndex, 0);
                auto presentQueue = backend->Device().getQueue(presentQueueIndex, 0);

                backend->Queues().Add(graphicsQueue, QueueOperation::GRAPHICS, graphicsQueueIndex);
                backend->Queues().Add(presentQueue, QueueOperation::PRESENT, presentQueueIndex);
            
                TUR_LOG_DEBUG("Initialized Vulkan Logical Device");
                TUR_LOG_DEBUG("Using Graphics Queue Family: {}", graphicsQueueIndex);
                TUR_LOG_DEBUG("Using Present Queue Family: {}", presentQueueIndex);
            }

            // Swapchain:
            {
                auto& physicalDevice = backend->PhysicalDevice();
                auto& surface = backend->SurfaceKHR();
                auto& device = backend->Device();
                auto& queues = backend->Queues();

                auto& capabilities = QuerySurfaceCapabilities(physicalDevice, surface);
                auto& surfaceFormats = QuerySurfaceFormats(physicalDevice, surface);
                auto& presentModes = QuerySurfacePresentModes(physicalDevice, surface);

                #ifdef TUR_DEBUG
                    DisplaySurfaceCapabilities(capabilities);
                    DisplaySurfaceFormats(surfaceFormats);
                    DisplayPresentModes(presentModes);
                #endif
    
                swapchainBuilder.SetArguments(surface, physicalDevice, device, queues)
                                .Prepare();

                auto swapchainResult = swapchainBuilder.Create();
                if (swapchainResult.has_value())
                    backend->Swapchain() = swapchainResult.value();

                // Image & Image Views:
                swapchainFrameBuilder.Build(device, backend->Swapchain());
            }
        }
    };
}