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
    PipelineOpenGL::PipelineOpenGL(const PipelineDescriptor& descriptor)
    {
		// TODO: Allow compute shaders.

		ShaderOpenGL* vertexShader = static_cast<ShaderOpenGL*>(descriptor.vertexShader);
		ShaderOpenGL* tessControlShader = static_cast<ShaderOpenGL*>(descriptor.tesselationControlShader);
		ShaderOpenGL* tessEvalShader = static_cast<ShaderOpenGL*>(descriptor.tesselationEvaluationShader);
		ShaderOpenGL* geometryShader = static_cast<ShaderOpenGL*>(descriptor.geometryShader);
		ShaderOpenGL* fragmentShader = static_cast<ShaderOpenGL*>(descriptor.fragmentShader);

		GLID vertexShaderID = vertexShader->GetID();
		GLID fragmentShaderID = fragmentShader->GetID();

		TUR_ASSERT(vertexShaderID != InvalidGLID, "Invalid vertex shader provided.");
		TUR_ASSERT(fragmentShaderID != InvalidGLID, "Invalid fragment shader provided.");

		// Optional Shaders:
		GLID tessControlShaderID = InvalidGLID;
		GLID tessEvalShaderID = InvalidGLID;
		GLID geometryShaderID = InvalidGLID;

		if (tessControlShader)
			tessControlShaderID = tessControlShader->GetID();

		if (tessEvalShader)
			tessEvalShaderID = tessEvalShader->GetID();

		if (geometryShader)
			geometryShaderID = geometryShader->GetID();

		// Creation:
		m_ID = glCreateProgram();

		glAttachShader(m_ID, vertexShaderID);
		glAttachShader(m_ID, fragmentShaderID);

		if (tessControlShaderID != InvalidGLID)
			glAttachShader(m_ID, tessControlShaderID);

		if (tessEvalShaderID != InvalidGLID)
			glAttachShader(m_ID, tessEvalShaderID);

		if (geometryShaderID != InvalidGLID)
			glAttachShader(m_ID, geometryShaderID);

		glLinkProgram(m_ID);
		CheckProgramLinkErrors(m_ID);

		// Cleanup:
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);

		if (tessControlShaderID != InvalidGLID)
			glDeleteShader(tessControlShaderID);

		if (tessEvalShaderID != InvalidGLID)
			glDeleteShader(tessEvalShaderID);

		if (geometryShaderID != InvalidGLID)
			glDeleteShader(geometryShaderID);
    }
}

