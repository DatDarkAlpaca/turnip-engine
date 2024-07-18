#pragma once
#include <numeric>
#include <limits>

namespace tur
{
	constexpr uint64_t InvalidHandle = std::numeric_limits<uint64_t>::max();

	struct AssetHandle
	{
	public:
		explicit AssetHandle(uint64_t handle = InvalidHandle) : handle(handle) { }

	public:
		operator uint64_t() const { return handle; }

	public:
		uint64_t handle = InvalidHandle;
	};
}