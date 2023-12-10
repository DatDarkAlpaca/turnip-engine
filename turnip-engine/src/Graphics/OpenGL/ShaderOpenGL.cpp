#include "pch.h"
#include "ShaderOpenGL.h"

#define TUR_GL_LOG_BUFFER_SIZE 1 << 11

namespace tur
{
    static constexpr GLID GetShaderTypeValue(ShaderType type)
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
                TUR_LOG_CRITICAL("OpenGL 4.5 does not support the following shader type: {}");
        }

        return InvalidGLID;
    }

    static void CheckCompileErrors(GLID shader, ShaderType type)
    {
        int success;
        char infoLog[TUR_GL_LOG_BUFFER_SIZE];

        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, TUR_GL_LOG_BUFFER_SIZE, NULL, infoLog);
            TUR_LOG_ERROR("Failed to compile {} Shader: {}", GetShaderTypeName(type), infoLog);
        }
    }

    static void CheckProgramLinkErrors(GLID program)
    {
        int success;
        char infoLog[TUR_GL_LOG_BUFFER_SIZE];

        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(program, TUR_GL_LOG_BUFFER_SIZE, NULL, infoLog);
            TUR_LOG_ERROR("Failed to link shader program: {}", infoLog);
        }
    }
}

namespace tur
{
	ShaderOpenGL::ShaderOpenGL(const std::vector<ShaderDescriptor>& shaderDescriptors)
	{
        m_ID = glCreateProgram();

        std::vector<unsigned int> shaderIDs;
        for (const auto& [filepath, type] : shaderDescriptors)
        {
            // Step 1: File reading
            std::string shaderCode;
            std::ifstream shaderFile;
            shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

            try
            {
                shaderFile.open(filepath);
                std::stringstream shaderStream;

                shaderStream << shaderFile.rdbuf();

                shaderFile.close();

                shaderCode = shaderStream.str();
            }
            catch (const std::ifstream::failure& e)
            {
                TUR_LOG_ERROR("Failed to read shader file at '{}': {}", filepath, e.what());
                continue;
            }

            // Step 2: Compiling
            const char* cShaderCode = shaderCode.c_str();
            GLID shaderID;

            shaderID = glCreateShader(GetShaderTypeValue(type));
            glShaderSource(shaderID, 1, &cShaderCode, NULL);
            glCompileShader(shaderID);
            CheckCompileErrors(shaderID, type);

            glAttachShader(m_ID, shaderID);

            shaderIDs.push_back(shaderID);
        }

        glLinkProgram(m_ID);
        CheckProgramLinkErrors(m_ID);

        for (const auto& shaderID : shaderIDs)
            glDeleteShader(shaderID);
	}
}
