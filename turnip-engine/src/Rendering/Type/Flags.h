#pragma once

namespace tur
{
    enum class ClearFlags
    {
        COLOR               = 1 << 0,
        DEPTH               = 1 << 1,
        STENCIL             = 1 << 2,
        COLOR_DEPTH         = COLOR | DEPTH,
        DEPTH_STENCIL       = DEPTH | STENCIL,
        ALL                 = COLOR | DEPTH | STENCIL,
    };
}