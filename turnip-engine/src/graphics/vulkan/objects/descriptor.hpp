#pragma once
#include <vulkan/vulkan.hpp>
#include "graphics/objects/descriptor.hpp"

namespace tur::vulkan
{
    struct Descriptor
    {
        vk::DescriptorPool descriptorPool;
        vk::DescriptorSetLayout setLayout;
    };

    struct DescriptorSet
    {
        vk::DescriptorSet set;
        descriptor_handle descriptorHandle;
    };
}

namespace tur::vulkan
{
    constexpr inline vk::DescriptorType get_descriptor_type(DescriptorType type)
    {
        switch (type)
        {
            case DescriptorType::COMBINED_IMAGE_SAMPLER:
                return vk::DescriptorType::eCombinedImageSampler;

            case DescriptorType::UNIFORM_BUFFER:
                return vk::DescriptorType::eUniformBuffer;

            case DescriptorType::STORAGE_BUFFER:
                return vk::DescriptorType::eStorageBuffer;
        }

        TUR_LOG_CRITICAL("Invalid Descriptor Type: {}", static_cast<u32>(type));
        return vk::DescriptorType::eUniformBuffer;
    }
}