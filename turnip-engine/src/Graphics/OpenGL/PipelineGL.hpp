#pragma once
#include "Graphics/Base/Pipeline.hpp"
#include "CommonGL.hpp"

namespace tur::gl
{
    inline void CheckProgramLinkErrors(uint32_t program)
    {
        constexpr uint64_t BufferSize = 256;

        int success;
        char infoLog[BufferSize];

        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(program, BufferSize, NULL, infoLog);
            TUR_LOG_ERROR("Failed to link shader program: {}", infoLog);
        }
    }
}