#pragma once
#include <vector>

#include "common.hpp"
#include "graphics/type/types.hpp"
#include "graphics/objects/shader.hpp"

namespace tur
{
    enum class FrontFace
    {
        COUNTER_CLOCKWISE,
        CLOCKWISE
    };

    enum class CullMode : u32
    {
        NONE            = 1 << 0,
        FRONT           = 1 << 1,
        BACK            = 1 << 2,
        FRONT_AND_BACK  = 1 << 3
    };

    inline u32 operator& (CullMode lhs, CullMode rhs)
    {
        return static_cast<u32>(lhs) & static_cast<u32>(rhs);
    }

    inline u32 operator| (CullMode lhs, CullMode rhs)
    {
        return static_cast<u32>(lhs) | static_cast<u32>(rhs);
    }

    enum class PolygonMode
    {
        FILL,
        LINE,
        POINT,
        FILL_RECTANGLE
    };

    enum class PrimitiveTopology
    {
        POINTS,

        LINES,
        LINE_STRIPS,

        TRIANGLES,
        TRIANGLE_STRIPS,
        TRIANGLE_FANS,

        LINES_WITH_ADJACENCY,
        LINE_STRIPS_WITH_ADJACENCY,

        TRIANGLES_WITH_ADJACENCY,
        TRIANGLE_STRIPS_WITH_ADJACENCY,

        PATCHES,
    };

    enum class InputRate
    {
        VERTEX,
        INSTANCE
    };

    enum class AttributeFormat
    {
        R32_SFLOAT,             // FLOAT
        R64_SFLOAT,             // DOUBLE
        R32G32_SFLOAT,          // VEC2
        R32G32B32_SFLOAT,       // VEC3
        R32G32B32A32_SFLOAT,    // VEC3
        R32G32_SINT,            // IVEC2
        R32G32B32A32_UINT,      // VEC4
    };

    enum class PipelineStage
    {
        NONE                = 0,
        VERTEX_STAGE        = 1 << 0,
        FRAGMENT_STAGE      = 1 << 2,
        ALL                 = VERTEX_STAGE | FRAGMENT_STAGE
    };
}

namespace tur
{
    struct PushConstant
    {
        u32 offset;
        u32 byteSize;
        PipelineStage stages;
    };
    
    struct PipelineLayout
    {
    public:
        void add_push_constant(const PushConstant& pushConstant)
        {
            pushConstants.push_back(pushConstant);
        }

    public:
        std::vector<PushConstant> pushConstants;
    };
}


namespace tur
{
    using pipeline_handle = handle_type;

    struct BindingDescription
    {
        u32 binding = 0;
        u32 stride  = 0;
        InputRate inputRate = InputRate::VERTEX;
    };

    struct Attribute
    {
        u32 binding  = 0;
        u32 location = 0;
        AttributeFormat format = AttributeFormat::R32_SFLOAT;
        u32 offset = 0;
        bool normalized = false;
    };

    struct VertexInputDescriptor
    {
        std::vector<BindingDescription> bindings;
        std::vector<Attribute> attributes;
    };

    struct InputAssemblyDescriptor
    {
        PrimitiveTopology topology = PrimitiveTopology::TRIANGLES;
        bool primitiveRestartEnable = false;
    };

    struct RasterizerDescriptor
    {
        FrontFace frontFace = FrontFace::COUNTER_CLOCKWISE;
        PolygonMode polygonMode = PolygonMode::FILL;
        CullMode cullMode = CullMode::FRONT;

        float lineWidth = 1.0f;

        bool discardFragments = false;
        bool enableDepthBias = false;
        bool clampDepth = false;
    };

    struct PipelineDescriptor
    {
        // Dynamic States: Scissor and Viewport
        VertexInputDescriptor vertexInputStage;
        InputAssemblyDescriptor inputAssemblyStage;
        RasterizerDescriptor rasterizerStage;

        PipelineLayout pipelineLayout;

        shader_handle vertexShader = invalid_handle;
        shader_handle tesselationControlShader = invalid_handle;
        shader_handle tesselationEvaluationShader = invalid_handle;
        shader_handle geometryShader = invalid_handle;
        shader_handle fragmentShader = invalid_handle;
    };
}