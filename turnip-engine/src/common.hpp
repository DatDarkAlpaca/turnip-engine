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
	using u8  = std::uint8_t;
	using u16 = std::uint16_t;
	using u32 = std::uint32_t;
	using u64 = std::uint64_t;

	using i8 = int8_t;
	using i16 = int16_t;
	using i32 = int32_t;
	using i64 = int64_t;

	using f32 = float;
	using f64 = double;

	using handle_type = u32;
	constexpr handle_type invalid_handle = std::numeric_limits<handle_type>::max();

	constexpr glm::uvec2 invalid_size = glm::uvec2(
		std::numeric_limits<handle_type>::max(),
		std::numeric_limits<handle_type>::max()
	);
}