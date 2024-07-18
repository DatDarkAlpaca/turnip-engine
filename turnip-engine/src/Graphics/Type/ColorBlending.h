#pragma once

namespace tur
{
    enum class BlendFactor
    {
        ZERO = 0,
        ONE,
        SRC_COLOR,
        ONE_MINUS_SRC_COLOR,
        DST_COLOR,
        ONE_MINUS_DST_COLOR,
        SRC_ALPHA,
        ONE_MINUS_SRC_ALPHA,
        DST_ALPHA,
        ONE_MINUS_DST_ALPHA,
        CONSTANT_COLOR,
        ONE_MINUS_CONSTANT_COLOR,
        CONSTANT_ALPHA,
        ONE_MINUS_CONSTANT_ALPHA,
        SRC_ALPHA_SATURATE,
        SRC1_COLOR,
        ONE_MINUS_SRC1_COLOR,
        SRC1_ALPHA,
        ONE_MINUS_SRC1_ALPHA,
    };

    enum class BlendOperation
    {
        ADD = 0,
        SUBTRACT,
        REVERSE_SUBTRACT,
        MIN,
        MAX,
        ZERO_EXT,
        SRC_EXT,
        DST_EXT,
        SRC_OVER_EXT,
        DST_OVER_EXT,
        SRC_IN_EXT,
        DST_IN_EXT,
        SRC_OUT_EXT,
        DST_OUT_EXT,
        SRC_ATOP_EXT,
        DST_ATOP_EXT,
        XOR_EXT,
        MULTIPLY_EXT,
        SCREEN_EXT,
        OVERLAY_EXT,
        DARKEN_EXT,
        LIGHTEN_EXT,
        COLORDODGE_EXT,
        COLORBURN_EXT,
        HARDLIGHT_EXT,
        SOFTLIGHT_EXT,
        DIFFERENCE_EXT,
        EXCLUSION_EXT,
        INVERT_EXT,
        INVERT_RGB_EXT,
        LINEARDODGE_EXT,
        LINEARBURN_EXT,
        VIVIDLIGHT_EXT,
        LINEARLIGHT_EXT,
        PINLIGHT_EXT,
        HARDMIX_EXT,
        HSL_HUE_EXT,
        HSL_SATURATION_EXT,
        HSL_COLOR_EXT,
        HSL_LUMINOSITY_EXT,
        PLUS_EXT,
        PLUS_CLAMPED_EXT,
        PLUS_CLAMPED_ALPHA_EXT,
        PLUS_DARKER_EXT,
        MINUS_EXT,
        MINUS_CLAMPED_EXT,
        CONTRAST_EXT,
        INVERT_OVG_EXT,
        RED_EXT,
        GREEN_EXT,
        BLUE_EXT,
    };

    enum class LogicOperation
    {
        CLEAR,
        AND,
        AND_REVERSE,
        COPY,
        AND_INVERTED,
        NO_OP,
        XOR,
        OR,
        NOR,
        EQUIVALENT,
        INVERT,
        OR_REVERSE,
        COPY_INVERTED,
        OR_INVERTED,
        NAND,
        SET
    };

    enum class ColorComponents : uint32_t
    {
        R = 1 << 0,
        G = 1 << 1,
        B = 1 << 2,
        A = 1 << 3
    };

    inline ColorComponents operator& (ColorComponents lhs, ColorComponents rhs)
    {
        return (ColorComponents)(static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs));
    }

    inline ColorComponents operator| (ColorComponents lhs, ColorComponents rhs)
    {
        return (ColorComponents)(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
    }
}