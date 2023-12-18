#pragma once

namespace tur
{
    enum class ColorBlendingOperation
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