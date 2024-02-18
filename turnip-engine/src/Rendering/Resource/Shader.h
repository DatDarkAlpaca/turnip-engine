#pragma once
#include "Common.h"
#include <numeric>
#include <string>

namespace tur
{
	enum class ShaderType
	{
		NONE = 0,
		VERTEX,
        TESSELATION_CONTROL,
        TESSELATION_EVALUATION,
		GEOMETRY,
		FRAGMENT,
		COMPUTE,
	};

	struct ShaderDescriptor
	{
		std::string filepath;
		ShaderType type;
	};

	constexpr inline const char* GetShaderTypeName(ShaderType type)
	{
		switch (type)
		{
			case ShaderType::NONE: 
				return "INVALID";

			case ShaderType::VERTEX: 
				return "VERTEX";

			case ShaderType::TESSELATION_CONTROL: 
				return "TESSELATION_CONTROL";

			case ShaderType::TESSELATION_EVALUATION: 
				return "TESSELATION_EVALUATION";

			case ShaderType::GEOMETRY: 
				return "GEOMETRY";

			case ShaderType::FRAGMENT: 
				return "FRAGMENT";

			case ShaderType::COMPUTE: 
				return "COMPUTE";

			default:
				return "UNKNOWN";
		}

		return "UNKNOWN";
	}

	enum class ShaderHandle : uint32_t { INVALID = InvalidHandle };
}