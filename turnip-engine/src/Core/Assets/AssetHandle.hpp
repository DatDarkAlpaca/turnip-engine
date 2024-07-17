#pragma once
#include <numeric>
#include <limits>

namespace tur
{
	constexpr uint32_t InvalidHandle = std::numeric_limits<uint32_t>::max();

	struct AssetHandle
	{
	public:
		explicit AssetHandle(uint32_t handle = InvalidHandle) : handle(handle) { }

	public:
		operator uint32_t() const { return handle; }

	public:
		uint32_t handle = InvalidHandle;
	};
}