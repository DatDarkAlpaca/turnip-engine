#pragma once
#include "pch.h"

namespace tur
{
	struct Attribute
	{
		U32 location = 0;
		U32 size;
		U32 type;
		bool normalized = false;
	};

	inline size_t GetTypeSize(U32 type)
	{
		if (type == GL_FLOAT)
			return sizeof(float);

		TUR_CORE_WARN("An invalid type has been passed as argument to getTypeSize()");
		return 0;
	}
}
