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

	constexpr inline const char* GetShaderTypeName(ShaderType type)
	{
		switch (type)
		{
			case ShaderType::INVALID: return "INVALID";
			case ShaderType::VERTEX: return "VERTEX";
			case ShaderType::TESSELATION_CONTROL: return "TESSELATION_CONTROL";
			case ShaderType::TESSELATION_EVALUATION: return "TESSELATION_EVALUATION";
			case ShaderType::GEOMETRY: return "GEOMETRY";
			case ShaderType::FRAGMENT: return "FRAGMENT";
			case ShaderType::COMPUTE: return "COMPUTE";
			case ShaderType::RAY_GENERATION: return "RAY_GENERATION";
			case ShaderType::RAY_INTERSECTION: return "RAY_INTERSECTION";
			case ShaderType::RAY_ANY_HIT: return "RAY_ANY_HIT";
			case ShaderType::RAY_CLOSEST_HIT: return "RAY_CLOSEST_HIT";
			case ShaderType::RAY_MISS: return "RAY_MISS";
			case ShaderType::CALLABLE: return "CALLABLE";
			case ShaderType::TASK: return "TASK";
			case ShaderType::MESH: return "MESH";
			default:
				return "UNKNOWN";
		}

		return "UNKNOWN";
	}

	struct ShaderDescriptor
	{
		std::string filepath;
		ShaderType type;
	};

	class IShader
	{
	public:
		virtual ~IShader() = default;
	};
}