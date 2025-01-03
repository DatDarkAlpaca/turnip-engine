#pragma once
#include "Graphics/Base/Shader.hpp"
#include "CommonGL.hpp"

namespace tur::gl
{
    inline void CheckCompileErrors(gl_handle shader, ShaderType type)
    {
        constexpr uint64_t BufferSize = 256;

        int success;
        char infoLog[BufferSize];

        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, BufferSize, NULL, infoLog);
            TUR_LOG_ERROR("Failed to compile {} Shader: {}", GetShaderTypeName(type), infoLog);
        }
    }

    constexpr inline gl_handle GetShaderType(ShaderType type)
    {
        switch (type)
        {
            case ShaderType::VERTEX:
                return GL_VERTEX_SHADER;

            case ShaderType::TESSELATION_CONTROL:
                return GL_TESS_CONTROL_SHADER;

            case ShaderType::TESSELATION_EVALUATION:
                return GL_TESS_EVALUATION_SHADER;

            case ShaderType::GEOMETRY:
                return GL_GEOMETRY_SHADER;

            case ShaderType::FRAGMENT:
                return GL_FRAGMENT_SHADER;

            case ShaderType::COMPUTE:
                return GL_COMPUTE_SHADER;

            default:
                TUR_LOG_CRITICAL("OpenGL does not support the following shader type: {}", (uint32_t)type);
        }

        return invalid_handle;
    }
}