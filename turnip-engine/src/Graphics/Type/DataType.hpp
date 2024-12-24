#pragma once

namespace tur
{
	enum class DataType
	{
		UNKNOWN,				
		R32_UINT,				// unsigned 32-bit int
		R32_SINT,				// signed 32-bit int
		R32_SFLOAT,				// signed float
		R64_SFLOAT,				// signed double
		R32G32_SFLOAT,			// signed 32-bit vec2
		R32G32B32_SFLOAT,		// signed 32-bit vec3
		R32G32B32A32_SFLOAT,	// signed 32-bit vec4
	};
}