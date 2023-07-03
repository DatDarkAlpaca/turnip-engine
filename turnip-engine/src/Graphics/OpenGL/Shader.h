#pragma once
#include "pch.h"
#include "ShaderHelpers.h"
#include "GraphicsCommon.h"

namespace tur
{
	class Shader
	{
	public:
		Shader(const std::vector<ShaderStructure>& shaders);

		Shader() = default;

		~Shader();

	public:
		void Initialize(const std::vector<ShaderStructure>& shaders);

		void Destroy();

	public:
		U32 GetUniform(const std::string& uniformName) const;

	public:
		void SetFloat(const char* name, float value);

		void SetInteger(const char* name, int value);

		void SetVector2f(const char* name, const glm::vec2& value);

		void SetVector3f(const char* name, const glm::vec3& value);

		void SetVector4f(const char* name, const glm::vec4& value);

		void SetMatrix4f(const char* name, glm::mat4 matrix);

	public:
		inline Shader& Bind() { glUseProgram(m_ID); return *this; }

		inline void Unbind() const { glUseProgram(0); }

	public:
		std::unordered_map<std::string, U32> m_Uniforms;
		unsigned int m_ID = TUR_GL_INVALID;
	};
}