#pragma once
#include <string>

namespace tur
{
	enum class GraphicsAPI 
	{ 
		NONE = -1, 
		OPENGL = 0 
	};

#ifdef TUR_DEBUG
	inline std::string GetGraphicsAPIString(GraphicsAPI api)
	{
		switch (api)
		{
			case GraphicsAPI::NONE: return "NONE";
			case GraphicsAPI::OPENGL: return "OPENGL";
		}

		return "UNKNOWN";
	}
#endif
}