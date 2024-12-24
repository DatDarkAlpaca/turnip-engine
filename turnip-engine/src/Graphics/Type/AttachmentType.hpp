#pragma once
#include <numeric>

namespace tur
{
	enum class AttachmentType : uint32_t
	{
		NONE = 0,
		COLOR,
		DEPTH,
		STENCIL,
		DEPTH_STENCIL,
		INPUT,
		RESOLVE,
	};
}