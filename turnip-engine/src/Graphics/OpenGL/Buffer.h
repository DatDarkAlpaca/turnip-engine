#pragma once
#include <glad/glad.h>
#include <numeric>

#include "Util/State/State.h"
#include "Rendering/Resource/Buffer.h"

namespace tur::gl
{
	struct Buffer
	{
		State<UsageFlag> usageFlags;
		uint32_t id;
	};
}