#pragma once
#include <string>
#include "common.hpp"

namespace tur
{
	struct InstancedQuadRendererInformation
	{
		std::string vertexFilepath;
		std::string fragmentFilepath;

		u32 maxInstanceCount = 0;
		u32 textureCount = 0;
		u32 textureWidth = 0;
		u32 textureHeight = 0;
	};
}