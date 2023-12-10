#include "pch.h"
#include "Shader.h"

namespace tur
{
	const char* GetShaderTypeName(ShaderType type)
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
		
		return nullptr;
	}

	tur_shared<Shader> Shader::Create(const std::vector<ShaderDescriptor>& descriptions)
	{


		return tur_shared<Shader>();
	}
	
}
