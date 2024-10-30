#include "pch.hpp"
#include "VulkanInitializer.hpp"

#include "GraphicsLayerVulkan.hpp"
#include "Builders/RenderpassBuilder.hpp"

namespace tur::vulkan
{
    IVulkanInitializer::IVulkanInitializer(const ConfigData& configData)
        : configData(configData)
    {

    }
}

namespace tur::vulkan
{
	DefaultVulkanInitializer::DefaultVulkanInitializer(const ConfigData& configData, platform::Window& window)
        : IVulkanInitializer(configData)
        , r_Window(window)
    {
        
    }

    void DefaultVulkanInitializer::Initialize(GraphicsLayerVulkan& graphicsLayer)
    {
        using namespace vulkan;

        auto applicationData = configData.applicationSpecification;
        auto graphicsData = configData.graphicsSpecifications;
        auto vulkanData = configData.vulkanArguments;

        bool enablePresentation = vulkanData.enablePresentation;

        // Instance:
        {
            instanceBuilder
                .SetAppName(applicationData.applicationName)
                .SetApplicationVersion(
                    applicationData.versionMajor,
                    applicationData.versionMinor,
                    applicationData.versionPatch,
                    applicationData.versionVariant)
                .SetEngineName("TurnipEngine")
                .SetAPIVersion(
                    graphicsData.versionMajor,
                    graphicsData.versionMinor,
                    graphicsData.versionPatch,
                    graphicsData.versionVariant)
                .AddLayers(vulkanData.layers)
                .AddExtensions(vulkanData.extensions)
                .UseDebugMessenger(vulkanData.useDebugMessenger);

            if (vulkanData.addValidationLayer)
                instanceBuilder.AddValidationLayer();

            if (vulkanData.addDebugExtensions)
                instanceBuilder.AddDebugUtilsExtension();

            auto instanceOutputResult = instanceBuilder.Build();

            if (!instanceOutputResult.has_value())
                TUR_LOG_CRITICAL("Vulkan Initializer: Failed to initialize instance");

            graphicsLayer.SetInstanceObject(instanceOutputResult.value());
        }

        const auto& instance = graphicsLayer.instanceObject.instance;

        // Surface:
        {
            auto surface = platform::vulkan::GetVulkanSurface(instance, &r_Window);
            graphicsLayer.SetSurfaceObject(surface);
        }

        const auto& surface = graphicsLayer.surfaceObject.surface;

        // Physical Device:
        PhysicalDeviceObject physicalDeviceOutput;
        {
            physicalDeviceSelector
                .SetConfigData(configData)
                .SetInstanceObject(graphicsLayer.instanceObject)
                .SetSurfaceObject(graphicsLayer.surfaceObject);

            physicalDeviceOutput = physicalDeviceSelector.Select();
            graphicsLayer.SetPhysicalDeviceObject(physicalDeviceOutput);
        }

        const auto& physicalDevice = graphicsLayer.physicalDeviceObject.physicalDevice;

        // Queues:
        uint32_t presentQueueIndex  = invalid_queue_index, graphicsQueueIndex = invalid_queue_index;
        {
            auto queueInfo = GetQueueFamilyInformation(physicalDevice, surface);

            for (const auto& queueFamily : queueInfo)
            {
                if (GetQueueFamilySupports(queueFamily, QueueOperation::PRESENT))
                    presentQueueIndex = queueFamily.familyIndex;

                if (GetQueueFamilySupports(queueFamily, QueueOperation::GRAPHICS))
                    graphicsQueueIndex = queueFamily.familyIndex;
            }
        }

        // Logical Device:
        {
            logicalDeviceBuilder
                .SetConfigData(configData)
                .SetInstanceObject(graphicsLayer.instanceObject)
                .SetPhysicalDeviceObject(graphicsLayer.physicalDeviceObject);

            if (enablePresentation)
            {
                logicalDeviceBuilder.PrepareQueueInfo(presentQueueIndex, 1.0f);

                if (graphicsQueueIndex != presentQueueIndex)
                    logicalDeviceBuilder.PrepareQueueInfo(graphicsQueueIndex, 1.0f);
            }
                
            auto logicalDeviceResult = logicalDeviceBuilder.Create();
            if (!logicalDeviceResult.has_value())
                TUR_LOG_CRITICAL("Vulkan Initializer: Failed to create logical device");

            // Logical Device:
            graphicsLayer.SetLogicalDeviceObject(logicalDeviceResult.value());
        }

        const auto& logicalDevice = graphicsLayer.logicalDeviceObject.device;

        // Queues:
        {
            auto graphicsQueue = logicalDevice.getQueue(graphicsQueueIndex, 0);
            auto presentQueue = logicalDevice.getQueue(presentQueueIndex, 0);

            graphicsLayer.deviceQueues.Add(graphicsQueue, graphicsQueueIndex, QueueOperation::GRAPHICS);
            graphicsLayer.deviceQueues.Add(presentQueue, presentQueueIndex, QueueOperation::PRESENT);
        }

        const auto& queues = graphicsLayer.deviceQueues;
        
        // VMA Allocator:
        {
            vmaBuilder.SetArguments(instance, physicalDevice, logicalDevice);
            graphicsLayer.SetAllocator(vmaBuilder.Build());
        }

        // Swapchain:
        {
            // TODO: configuration data.

            auto& capabilities = QuerySurfaceCapabilities(physicalDevice, surface);
            auto& surfaceFormats = QuerySurfaceFormats(physicalDevice, surface);
            auto& presentModes = QuerySurfacePresentModes(physicalDevice, surface);

            swapchainBuilder
                .SetArguments(surface, physicalDevice, logicalDevice, queues)
                .Prepare();

            auto swapchainResult = swapchainBuilder.Create();
            if (swapchainResult.has_value())
                graphicsLayer.SetSwapchainObject(swapchainResult.value());
        }

        const auto& swapchain = graphicsLayer.swapchainObject;

        // Image & Image Views:
        {
            auto frames = frameBuilder.Build(logicalDevice, swapchain);
            graphicsLayer.SetFrames(frames);
        }

        auto& frames = graphicsLayer.frames;

        // Swapchain Frame Renderpass:
        {
            RenderpassObject renderpass;

            RenderpassDescriptor descriptor 
                = RenderpassDescriptor::CreateDefaultRenderpass({ swapchain.extent.width, swapchain.extent.height });

            vulkan::RenderpassBuilder renderpassBuilder(descriptor);
            renderpassBuilder.SetArguments(logicalDevice, swapchain);

            auto vulkanRenderpassResult = renderpassBuilder.Build();
            if(!vulkanRenderpassResult.has_value())
                TUR_LOG_CRITICAL("Vulkan Initializer: Failed to create swapchain renderpass");

            frames.renderpassObject = vulkanRenderpassResult.value();
        }

        // Swapchain Frame Commands:
        {
            //// Commands:
            //CommandPoolBuilder poolBuilder;
            //poolBuilder.SetArguments(logicalDevice, queues);
            //commandPool = poolBuilder.Build().value(); // TODO: check value

            //vulkan::CommandBufferBuilder commandBuilder;
            //commandBuilder.SetArguments(logicalDevice, commandPool);
        }

        // Swapchain Frame Framebuffers & Sync:
        {
            //// Frames:
            //vulkan::FramebufferBuilder frameBuilder;
            //frameBuilder.SetArguments(logicalDevice, swapchain.renderpass);

            //for (auto& frame : swapchain.frames)
            //{
            //    frame.framebuffer = frameBuilder.Create(frame.view, swapchain);
            //    frame.commandBuffer = commandBuilder.Build().value(); // TODO: check value

            //    SemaphoreBuilder semaphoreBuilder;
            //    FenceBuilder fenceBuilder;

            //    // Sync objects:
            //    frame.inFlightFence = fenceBuilder.Build(logicalDevice);
            //    frame.renderFinishedSemaphore = semaphoreBuilder.Build(logicalDevice);
            //    frame.imageAvailableSemaphore = semaphoreBuilder.Build(logicalDevice);
            //}
        }
    }
}