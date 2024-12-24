#pragma once
#include "Graphics/CommonGraphics.hpp"

namespace tur
{
    using texture_handle = handle_type;

    enum class TextureUsage 
    {
        SAMPLING,
        RENDER_TARGET,
        STORAGE,
    };

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

    struct TextureDescriptor 
    {
        uint32_t width;
        uint32_t height;
        uint32_t depth = 1;
        uint32_t mipLevels = 1;

        TextureFormat format;
        TextureUsage usage;
        TextureType type;

        bool generateMipmaps = false;

        WrapMode wrapS = WrapMode::REPEAT;
        WrapMode wrapT = WrapMode::REPEAT;
        WrapMode wrapR = WrapMode::REPEAT;
        FilterMode minFilter = FilterMode::NEAREST;
        FilterMode magFilter = FilterMode::NEAREST;
    };
}