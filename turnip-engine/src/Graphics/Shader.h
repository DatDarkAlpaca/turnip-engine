#pragma once
#include "Common.h"
#include <string>

namespace tur
{
	enum class ShaderType 
	{
		INVALID = 0,
		VERTEX,
		TESSELATION_CONTROL,
		TESSELATION_EVALUATION,
		GEOMETRY,
		FRAGMENT,
		COMPUTE,
		RAY_GENERATION,
		RAY_INTERSECTION,
		RAY_ANY_HIT,
		RAY_CLOSEST_HIT,
		RAY_MISS,
		CALLABLE,
		TASK,
		MESH
	};

	const char* GetShaderTypeName(ShaderType type);

	struct ShaderDescriptor
	{
		std::string filepath;
		ShaderType type;
	};

	class Shader
	{
	public:
		virtual ~Shader() = default;
	};
}