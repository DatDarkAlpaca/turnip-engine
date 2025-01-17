#pragma once
#include <numeric>
#include <limits>
#include <filesystem>
#include <glm/glm.hpp>

#include "core/defines.hpp"

#include "core/memory/memory.hpp"
#include "core/logger/logger.hpp"
#include "core/assert/assert.hpp"

#define TUR_ENGINE_NAME "TurnipEngine"

#define BIND(function, argument) std::bind(function, argument, std::placeholders::_1)

#define EXPOSE_PROPERTY(ClassName, SetterName, PropertyName)						\
	const ClassName& SetterName() const { return PropertyName; }					\
	ClassName& SetterName() { return PropertyName; }

namespace tur
{
	using handle_type = uint32_t;
	constexpr handle_type invalid_handle = std::numeric_limits<handle_type>::max();

	constexpr glm::uvec2 invalid_size = glm::uvec2(
		std::numeric_limits<handle_type>::max(),
		std::numeric_limits<handle_type>::max()
	);
}