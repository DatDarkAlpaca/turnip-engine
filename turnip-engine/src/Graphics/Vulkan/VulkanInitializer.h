#pragma once
#include "Common.h"
#include "RenderDeviceVK.h"
#include "Rendering/RenderInitializer.h"

#include "Graphics/GraphicsSpecification.h"

#include "Builders/InstanceBuilder.h"
#include "Builders/PhysicalDeviceBuilder.h"
#include "Builders/LogicalDeviceBuilder.h"
#include "Builders/VMABuilder.h"
#include "Builders/SwapchainBuilder.h"
#include "Builders/FrameBuilder.h"

namespace tur::vulkan
{
    class VulkanInitializer : public RenderInitializer
    {
    public:
        VulkanInitializer(const GraphicsSpecification& specification);

    public:
        virtual ~VulkanInitializer() = default;
        
    protected:
        VulkanInstanceBuilder instanceBuilder;
        PhysicalDeviceSelector physicalDeviceSelector;
        LogicalDeviceBuilder logicalDeviceBuilder;
        SwapchainBuilder swapchainBuilder;
        SwapchainFrameBuilder swapchainFrameBuilder;
        VMABuilder vmaBuilder;

    protected:
        GraphicsSpecification specification;
    };

    class DefaultVulkanInitializer final : public VulkanInitializer
    {
    public:
        DefaultVulkanInitializer(const GraphicsSpecification& specification);

    public:
        void Initialize(NON_OWNING RenderDevice* _device) override;
    };
}