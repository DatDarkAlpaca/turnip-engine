#include "pch.h"
#include "CommonGL.h"
#include "PipelineOpenGL.h"

namespace
{
    /*static void CheckProgramLinkErrors(GLID program)
    {
        int success;
        char infoLog[TUR_GL_LOG_BUFFER_SIZE];

        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(program, TUR_GL_LOG_BUFFER_SIZE, NULL, infoLog);
            TUR_LOG_ERROR("Failed to link shader program: {}", infoLog);
        }
    }*/
}

namespace tur
{
    PipelineOpenGL::PipelineOpenGL()
    {
    }
}

