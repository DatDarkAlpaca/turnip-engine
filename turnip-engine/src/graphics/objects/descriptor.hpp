#pragma once
#include "common.hpp"
#include "graphics/objects/pipeline_stage.hpp"

namespace tur
{
    enum class DescriptorType
    {
        INVALID = 0,
        COMBINED_IMAGE_SAMPLER,
        UNIFORM_BUFFER,
        STORAGE_BUFFER
    };
}

namespace tur
{
	using descriptor_handle = handle_type;

    struct DescriptorSetLayoutEntry
    {
        u32 binding = 0;
        u32 amount = 1;
        DescriptorType type = DescriptorType::INVALID;
        PipelineStage stage = PipelineStage::NONE;
    };

    using DescriptorSetLayoutDescriptor = std::vector<DescriptorSetLayoutEntry>;
}