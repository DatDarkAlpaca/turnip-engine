#include "pch.h"
#include "Shader.h"
#include "GraphicsCommon.h"

#include "File/ReadFile.h"
#include "Logger/Logger.h"
#include "Assert/Assert.h"

namespace
{
	using namespace tur;

#ifdef TUR_DEBUG
	static std::string GetShaderTypeString(ShaderType shaderType)
	{
		switch (shaderType)
		{
			case ShaderType::TUR_SHADER_VERTEX:
				return "Vertex";
			case ShaderType::TUR_SHADER_FRAGMENT:
				return "Fragment";
			case ShaderType::TUR_SHADER_GEOMETRY:
				return "Geometry";
			case ShaderType::TUR_SHADER_COMPUTE:
				return "Compute";
			case ShaderType::TUR_SHADER_TESS_CONTROL:
				return "Tesselation Control";
			case ShaderType::TUR_SHADER_TESS_EVAL:
				return "Tesselation Evaluation";
			default:
				return "Unknown Shader";
		}
	}
#endif

	static U32 CompileShader(const ShaderStructure& shader)
	{
		U32 shaderID = glCreateShader((U32)shader.shaderType);
		std::string shaderSource = ReadFile(shader.filepath);

		const char* c_shaderSource = shaderSource.c_str();
		glShaderSource(shaderID, 1, &c_shaderSource, NULL);
		glCompileShader(shaderID);

		return shaderID;
	}

	static void CheckCompileErrors(U32 shader, ShaderType type)
	{
		int success;
		char infoLog[1024];

		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);

			TUR_WRAP_DEBUG(TUR_CORE_ERROR("{} Shader Error:\n{}", GetShaderTypeString(type), infoLog));

			glDeleteShader(shader);
		}
	}

	static void CheckLinkErrors(unsigned int program)
	{
		int success;
		char infoLog[1024];

		glGetProgramiv(program, GL_LINK_STATUS, &success);

		if (!success)
		{
			glGetProgramInfoLog(program, 1024, nullptr, infoLog);
			TUR_CORE_ERROR("Linking Error:\n{}", infoLog);

			glDeleteProgram(program);
		}
	}

	static std::vector<std::pair<std::string, U32>> ListActiveUniforms(U32 program)
	{
		std::vector<std::pair<std::string, U32>> uniforms;

		int count;
		glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);

		const int maxBufferSize = 24;
		for (size_t i = 0; i < count; i++)
		{
			int length, size;
			unsigned int type;
			char name[maxBufferSize];
			glGetActiveUniform(program, (GLuint)i, maxBufferSize, &length, &size, &type, name);

			uniforms.push_back({ name, (U32)i });
		}

		return uniforms;
	}
}

namespace tur
{
	Shader::Shader(const std::vector<ShaderStructure>& shaders)
	{
		Initialize(shaders);
	}
	
	Shader::~Shader()
	{
		if (m_ID == TUR_GL_INVALID)
			return;
		
		TUR_CORE_WARN("Shader instance destructor called, but object was never destroyed. It is advised to use Destroy() beforehand.");
	}

	void Shader::Initialize(const std::vector<ShaderStructure>& shaders)
	{
		m_ID = glCreateProgram();

		std::vector<U32> compiledShaders;
		for (const auto& shader : shaders)
		{
			compiledShaders.push_back(CompileShader(shader));
			TUR_WRAP_DEBUG(CheckCompileErrors(compiledShaders.back(), shader.shaderType));

			glAttachShader(m_ID, compiledShaders.back());
		}

		glLinkProgram(m_ID);
		glValidateProgram(m_ID);
		TUR_WRAP_DEBUG(CheckLinkErrors(m_ID));

		// This probably adds a lot of overhead and makes the code slower than a simple for,
		// but I wanted to test it out. Code subject to change.
		std::for_each(std::execution::par, compiledShaders.begin(), compiledShaders.end(),
			[](U32 shader) { glDeleteShader(shader); });

		// Uniform caching:
		for (const auto& [uniform, location] : ListActiveUniforms(m_ID))
			m_Uniforms[uniform] = location;
	}

	void Shader::Destroy()
	{
		TUR_ASSERT(m_ID != TUR_GL_INVALID, "Attempted to destroy a non-initialized shader.");

		glDeleteProgram(m_ID);
		m_ID = TUR_GL_INVALID;
	}

	U32 Shader::GetUniform(const std::string& uniformName) const
	{
		if (m_Uniforms.find(uniformName) == m_Uniforms.end())
		{
			TUR_CORE_WARN("The uniform {} does not exist.", uniformName.data());
			return TUR_GL_INVALID;
		}

		return m_Uniforms.at(uniformName);
	}

	void Shader::SetFloat(const char* name, float value)
	{
		glUniform1f(GetUniform(name), value);
	}

	void Shader::SetInteger(const char* name, int value)
	{
		glUniform1i(GetUniform(name), value);
	}

	void Shader::SetVector2f(const char* name, const glm::vec2& value)
	{
		glUniform2f(GetUniform(name), value.x, value.y);
	}

	void Shader::SetVector3f(const char* name, const glm::vec3& value)
	{
		glUniform3f(GetUniform(name), value.x, value.y, value.z);
	}

	void Shader::SetVector4f(const char* name, const glm::vec4& value)
	{
		glUniform4f(GetUniform(name), value.x, value.y, value.z, value.w);
	}

	void Shader::SetMatrix4f(const char* name, glm::mat4 matrix)
	{
		glUniformMatrix4fv(GetUniform(name), 1, false, glm::value_ptr(matrix));
	}
}
