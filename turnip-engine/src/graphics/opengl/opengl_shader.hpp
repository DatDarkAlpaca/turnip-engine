#pragma once
#include <glad/glad.h>

#include "opengl_common.hpp"
#include "graphics/objects/shader.hpp"

namespace tur::gl
{
    struct Shader
    {
        gl_handle textureHandle;
    };
}

namespace tur::gl
{
    constexpr inline gl_handle get_shader_type(ShaderType type)
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
                TUR_LOG_CRITICAL("OpenGL does not support the following shader type: {}", (u32)type);
        }

        return invalid_handle;
    }

    inline void check_compile_error(gl_handle shader, ShaderType type)
    {
        constexpr u64 BufferSize = 256;

        int success;
        char infoLog[BufferSize];

        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, BufferSize, NULL, infoLog);
            TUR_LOG_ERROR("Failed to compile {} Shader: {}", get_shader_type(type), infoLog);
        }
    }
}