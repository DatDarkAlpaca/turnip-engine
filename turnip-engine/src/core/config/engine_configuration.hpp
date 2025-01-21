#pragma once
#include <nlohmann/json.hpp>
#include "common.hpp"

namespace tur
{
	struct EngineSpecification
	{
		u32 major = 1, minor = 0, patch = 0, variant = 0;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(
			EngineSpecification,
			major,
			minor,
			patch,
			variant
		);
	};
}