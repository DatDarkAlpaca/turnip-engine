#pragma once
#include <glad/glad.h>
#include <numeric>

#include "Rendering/Resource/Pipeline.h"
#include "ShaderGL.h"

namespace tur::gl
{
    static inline void CheckProgramLinkErrors(uint32_t program)
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

    struct PipelineGL
    {
        PipelineStateDescriptor state;
        uint32_t id;
    };

    inline void SetupPipelineShaders(gl::PipelineGL& pipeline, const std::vector<ShaderGL>& shaders)
    {
        pipeline.id = glCreateProgram();

        for (const auto& shader : shaders)
            glAttachShader(pipeline.id, shader.id);
       
        glLinkProgram(pipeline.id);
        CheckProgramLinkErrors(pipeline.id);

        for (const auto& shader : shaders)
            glDeleteShader(shader.id);
    }
}