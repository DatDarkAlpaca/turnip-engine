#pragma once
#include "Common.h"
#include "BackendVulkan.h"

#include "Instance.h"

namespace tur
{
    class IVulkanInitializer
    {
    public:
        explicit IVulkanInitializer(BackendVulkan* backend)
            : backend(backend) { }

        virtual ~IVulkanInitializer() = default;

    protected:
        NON_OWNING BackendVulkan* backend = nullptr;

    protected:
        vulkan::VulkanInstanceBuilder instanceBuilder;
    };

    class DefaultVulkanInitializer : public IVulkanInitializer
    {
    public:
        DefaultVulkanInitializer(BackendVulkan* backend)
            : IVulkanInitializer(backend)
        {
            instanceBuilder
                .SetAppName(backend->Properties().applicationName)
                .SetEngineName("TurnipEngine");
            
            Configure();
        }

    private:
        void Configure()
        {
            // Instance:
            {
                auto instanceOutputResult = instanceBuilder.Build();
                if (!instanceOutputResult.has_value())
                    TUR_LOG_CRITICAL("Vulkan Initializer: Failed to initialize instance");

                auto& instanceOutput = instanceOutputResult.value();

                backend->Instance() = instanceOutput.instanceHandle;
                backend->DebugMessenger() = instanceOutput.debugMessenger;

                instanceBuilder.DisplayVulkanAPIVersion();
                TUR_LOG_DEBUG("Initialized Vulkan Instance");
                TUR_LOG_DEBUG("Initialized Vulkan DebugMessenger");
            }
        }
    };
}