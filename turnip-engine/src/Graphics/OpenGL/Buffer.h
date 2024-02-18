#pragma once
#include <glad/glad.h>
#include <numeric>

#include "Rendering/Resource/Buffer.h"

namespace tur::gl
{
	struct Buffer
	{
		BindingFlag bindingFlag;
		uint32_t id;
	};
}