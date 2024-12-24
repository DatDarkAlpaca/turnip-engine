#pragma once
#include "CommonVulkan.hpp"
#include "Core/Config/ConfigData.hpp"

#include "Builders/InstanceBuilder.hpp"
#include "Builders/PhysicalDeviceBuilder.hpp"
#include "Builders/LogicalDeviceBuilder.hpp"
#include "Builders/VMABuilder.hpp"
#include "Builders/SwapchainBuilder.hpp"
#include "Builders/FrameBuilder.hpp"

#include "Platform/Platform.hpp"

namespace tur::vulkan
{
    class GraphicsLayerVulkan;

    class IVulkanInitializer
    {
    public:
        IVulkanInitializer(const ConfigData& configData);

    public:
        virtual ~IVulkanInitializer() = default;
        
    public:
        virtual void Initialize(GraphicsLayerVulkan& graphicsLayer) = 0;

    protected:
        InstanceBuilder instanceBuilder;
        PhysicalDeviceSelector physicalDeviceSelector;
        LogicalDeviceBuilder logicalDeviceBuilder;
        VMABuilder vmaBuilder;
        SwapchainBuilder swapchainBuilder;
        FrameBuilder frameBuilder;

    protected:
        ConfigData configData;
    };

    class DefaultVulkanInitializer final : public IVulkanInitializer
    {
    public:
        DefaultVulkanInitializer(const ConfigData& configData, platform::Window& window);

    public:
        void Initialize(GraphicsLayerVulkan& graphicsLayer) override;

    private:
        platform::Window& r_Window;
    };
}