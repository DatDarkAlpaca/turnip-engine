#pragma once
#include "pch.h"

namespace tur
{
	enum class GraphicsAPI_Type { NONE = 0, OPENGL, D3D11, D3D12, VULKAN, DEFAULT = OPENGL };

#ifdef TUR_DEBUG
	inline std::string GetGraphicsAPI_String(GraphicsAPI_Type type)
	{
		switch(type)
		{
			case GraphicsAPI_Type::NONE:	return "NONE";
			case GraphicsAPI_Type::OPENGL:	return "OPENGL";
			case GraphicsAPI_Type::D3D11:	return "D3D11";
			case GraphicsAPI_Type::D3D12:	return "D3D12";
			case GraphicsAPI_Type::VULKAN:	return "VULKAN";
			default:						return "UNKNOWN";
		}
	}
#endif
}