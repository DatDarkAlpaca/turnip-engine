#pragma once

namespace tur
{
    enum class DataFormat
    {
        UNDEFINED,
        R8_UNORM,
        R8_SNORM,

        R8_UINT,
        R8_SINT,
        R8_SRGB,

        R8_G8_UNORM,
        R8_G8_SNORM,

        R8_G8_UINT,
        R8_G8_SINT,
        R8_G8_SRGB,

        R8_G8_B8_UNORM,
        R8_G8_B8_SNORM,
        R8_G8_B8_UINT,
        R8_G8_B8_SINT,
        R8_G8_B8_SRGB,

        R8_G8_B8_A8_UNORM,
        R8_G8_B8_A8_SNORM,
        R8_G8_B8_A8_UINT,
        R8_G8_B8_A8_SINT,
        R8_G8_B8_A8_SRGB,

        R16_UNORM,
        R16_SNORM,
        R16_UINT,
        R16_SINT,

        R16_SFLOAT,
        R16_G16_UNORM,
        R16_G16_SNORM,
        R16_G16_UINT,
        R16_G16_SINT,
        R16_G16_SFLOAT,

        R16_G16_B16_UNORM,
        R16_G16_B16_SNORM,
        R16_G16_B16_UINT,
        R16_G16_B16_SINT,
        R16_G16_B16_SFLOAT,
        R16_G16_B16_A16UNORM,

        R16_G16_B16_A16_SNORM,
        R16_G16_B16_A16_UINT,
        R16_G16_B16_A16_SINT,
        R16_G16_B16_A16_SFLOAT,

        R32_UINT,
        R32_SINT,
        R32_SFLOAT,
        R32_G32_UINT,
        R32_G32_SINT,
        R32_G32_SFLOAT,
        R32_G32_B32_UINT,
        R32_G32_B32_SINT,
        R32_G32_B32_SFLOAT,
        R32_G32_B32_A32_UINT,
        R32_G32_B32_A32_SINT,
        R32_G32_B32_A32_SFLOAT,

        R64_UINT,
        R64_SINT,
        R64_SFLOAT,
        R64_G64_UINT,
        R64_G64_SINT,
        R64_G64_SFLOAT,
        R64_G64_B64_UINT,
        R64_G64_B64_SINT,
        R64_G64_B64_SFLOAT,
        R64_G64_B64_A64_UINT,
        R64_G64_B64_A64_SINT,
        R64_G64_B64_A64_SFLOAT,
    };
}