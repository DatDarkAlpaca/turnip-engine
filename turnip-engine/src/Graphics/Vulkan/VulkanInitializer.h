#pragma once
#include "Common.h"
#include "BackendVulkan.h"

#include "Instance.h"
#include "PhysicalDevice.h"
#include "LogicalDevice.h"
#include "Queues.h"
#include "Swapchain.h"

namespace tur
{
    class IVulkanInitializer
    {
    public:
        explicit IVulkanInitializer(BackendVulkan* backend)
            : backend(backend) 
        {

        }

        virtual ~IVulkanInitializer() = default;

    protected:
        NON_OWNING BackendVulkan* backend = nullptr;

    protected:
        vulkan::VulkanInstanceBuilder instanceBuilder;
        vulkan::PhysicalDeviceSelector physicalDeviceSelector;
        vulkan::LogicalDeviceBuilder logicalDeviceBuilder;
        vulkan::SwapchainBuilder swapchainBuilder;
    };

    class DefaultVulkanInitializer : public IVulkanInitializer
    {
    public:
        DefaultVulkanInitializer(BackendVulkan* backend)
            : IVulkanInitializer(backend)
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
            InstanceOutput instanceOutput;
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
                backend->SurfaceKHR() = platform::GetVulkanSurface(instanceOutput.instanceHandle, backend->GetWindow()->GetHandle());
            }

            // Physical Device:
            PhysicalDeviceOutput physicalDeviceOutput;
            {
                physicalDeviceSelector.SetInstance(instanceOutput)
                                      .SetSurface(backend->SurfaceKHR());

                physicalDeviceOutput = physicalDeviceSelector.Select();
                backend->PhysicalDevice() = physicalDeviceOutput.device;

                TUR_LOG_DEBUG("Selected GPU: {}", physicalDeviceOutput.device.getProperties().deviceName.data());
            }

            // Queue Creation:
            vk::DeviceQueueCreateInfo presentQueueInfo, graphicsQueueInfo;
            uint32_t presentQueueIndex, graphicsQueueIndex;
            std::vector<vk::Queue> queues;
            {
                physicalDeviceOutput.queueInformation;
                               
                for (const auto& queue : physicalDeviceOutput.queueInformation)
                {
                    if (GetQueueSupports(queue, QueueOperation::PRESENT))
                        presentQueueIndex = queue.familyIndex;

                    if (GetQueueSupports(queue, QueueOperation::GRAPHICS))
                        graphicsQueueIndex = queue.familyIndex;
                }

                presentQueueInfo = SelectQueue(presentQueueIndex);
                graphicsQueueInfo = SelectQueue(graphicsQueueIndex);
            }

            // Logical Device & Queue:
            {
                logicalDeviceBuilder.SetInstanceOutput(instanceOutput)
                                    .SetPhysicalDeviceOutput(physicalDeviceOutput);

                logicalDeviceBuilder.AddQueueInfo(presentQueueInfo)
                                    .AddQueueInfo(presentQueueInfo);

                backend->Device() = logicalDeviceBuilder.Create().value();
                backend->Queues().Add(backend->Device().getQueue(graphicsQueueIndex, 0), QueueOperation::GRAPHICS, graphicsQueueIndex);
                backend->Queues().Add(backend->Device().getQueue(presentQueueIndex, 0), QueueOperation::PRESENT, presentQueueIndex);
            
                TUR_LOG_DEBUG("Initialized Vulkan Logical Device");
                TUR_LOG_DEBUG("Using Graphics Queue: {}", graphicsQueueIndex);
                TUR_LOG_DEBUG("Using Present Queue: {}", presentQueueIndex);
            }

            // Swapchain:
            {
                auto& physicalDevice = backend->PhysicalDevice();
                auto& surface = backend->SurfaceKHR();
                auto& device = backend->Device();
                auto& queues = backend->Queues();

                auto& capabilities = QuerySurfaceCapabilities(physicalDevice, surface);
                auto& surfaceFormats = QuerySurfaceFormats(physicalDevice, surface);
                auto& presentMode = QuerySurfacePresentModes(physicalDevice, surface);

                #ifdef TUR_DEBUG
                    DisplaySurfaceCapabilities(capabilities);
                    DisplaySurfaceFormats(surfaceFormats);
                    DisplayPresentModes(presentMode);
                #endif
    
                swapchainBuilder.SetArguments(surface, physicalDevice, device, queues)
                                .Prepare();

                backend->Swapchain() = swapchainBuilder.Create();
            }
        }
    };
}