#include "pch.h"
#include "ShaderOpenGL.h"
#include "Util/File.h"

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
}

namespace tur
{
	ShaderOpenGL::ShaderOpenGL(const ShaderDescriptor& descriptor)
	{
        auto shaderContents = ReadFile(descriptor.filepath);

        std::string shaderCode;
        std::ifstream shaderFile;

        const char* cShaderCode = shaderCode.c_str();
        m_ID = glCreateShader(GetShaderTypeValue(descriptor.type));
        glShaderSource(m_ID, 1, &cShaderCode, NULL);
        glCompileShader(m_ID);

        CheckCompileErrors(m_ID, descriptor.type);
	}
}
