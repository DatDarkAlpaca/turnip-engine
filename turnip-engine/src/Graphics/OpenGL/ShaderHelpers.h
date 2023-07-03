#pragma once
#include "pch.h"

namespace tur
{
	enum class ShaderType
	{
		TUR_SHADER_NONE = 0,
		TUR_SHADER_VERTEX = GL_VERTEX_SHADER,
		TUR_SHADER_FRAGMENT = GL_FRAGMENT_SHADER,
		TUR_SHADER_GEOMETRY = GL_GEOMETRY_SHADER,
		TUR_SHADER_COMPUTE = GL_COMPUTE_SHADER,
		TUR_SHADER_TESS_CONTROL = GL_TESS_CONTROL_SHADER,
		TUR_SHADER_TESS_EVAL = GL_TESS_EVALUATION_SHADER,
	};

	struct ShaderStructure
	{
		const char* filepath;
		ShaderType shaderType;
	};
}