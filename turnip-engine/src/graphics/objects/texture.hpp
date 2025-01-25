#pragma once
#include "common.hpp"

namespace tur
{
	using texture_handle = handle_type;

    enum class TextureType
    {
        TEXTURE_2D,
        TEXTURE_3D,
        ARRAY_TEXTURE,
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

    // Represents a texture on the GPU
    enum class TextureFormat
    {
        RED,
        RG,
        RGB,
        RGBA,

        RGBA16SFloat,

        DEPTH_COMPONENT,
        DEPTH_STENCIL,

        B8G8R8A8_UNORM
    };

    // Represents data formats and is used for loading textures, rather than representing them on the GPU 
    enum class TextureDataFormat
    {
        RED,
        RG,
        RGB,
        RGBA,
        BGR,
        BGRA,

        RED_INTEGER,
        RG_INTEGER,
        RGB_INTEGER,
        RGBA_INTEGER,
        BGR_INTEGER,
        BGRA_INTEGER,
    };

    struct TextureDescriptor
    {
        // todo: array layers. 

        u32 width = 0;
        u32 height = 0;
        u32 depth = 1;
        u32 mipLevels = 1;

        TextureFormat format = TextureFormat::RGBA;
        TextureType type = TextureType::TEXTURE_2D;

        bool generateMipmaps = false;

        WrapMode wrapS = WrapMode::REPEAT;
        WrapMode wrapT = WrapMode::REPEAT;
        WrapMode wrapR = WrapMode::REPEAT;
        FilterMode minFilter = FilterMode::NEAREST;
        FilterMode magFilter = FilterMode::NEAREST;
    };
}