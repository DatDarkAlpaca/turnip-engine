#pragma once
#include "Common.h"
#include "BackendVulkan.h"

#include "Instance.h"
#include "PhysicalDevice.h"
#include "LogicalDevice.h"

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

            // Queues:
            vk::DeviceQueueCreateInfo presentQueueInfo, graphicsQueueInfo;
            {
                physicalDeviceOutput.queueInformation;
                uint32_t presentQueueIndex, graphicsQueueIndex;
                
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

            // Logical Device:
            {

            }
        }
    };
}