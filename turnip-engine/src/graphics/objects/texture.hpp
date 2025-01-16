#pragma once
#include "common.hpp"

namespace tur
{
	using texture_handle = handle_type;

    enum class TextureType
    {
        TEXTURE_2D,
        TEXTURE_3D,
        CUBE_MAP,
    };

    enum class WrapMode
    {
        REPEAT,
        MIRRORED_REPEAT,
        CLAMP_TO_EDGE,
        CLAMP_TO_BORDER
    };

    enum class FilterMode
    {
        NEAREST,
        LINEAR,
        BILINEAR,
        TRILINEAR
    };

    // TODO: extend.
    enum class TextureFormat
    {
        DEPTH_COMPONENT,
        DEPTH_STENCIL,
        RED,
        RG,
        RGB,
        RGBA
    };

    struct TextureDescriptor
    {
        uint32_t width = 0;
        uint32_t height = 0;
        uint32_t depth = 1;
        uint32_t mipLevels = 1;

        TextureFormat format;
        TextureType type;

        bool generateMipmaps = false;

        WrapMode wrapS = WrapMode::REPEAT;
        WrapMode wrapT = WrapMode::REPEAT;
        WrapMode wrapR = WrapMode::REPEAT;
        FilterMode minFilter = FilterMode::NEAREST;
        FilterMode magFilter = FilterMode::NEAREST;
    };
}