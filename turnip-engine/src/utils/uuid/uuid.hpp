#pragma once
#include <numeric>

#include "common.hpp"
#include "utils/random/random.hpp"

namespace tur
{
	class UUID
	{
	public:
		UUID()
			: m_UUID(generate_random<u64>())
		{

		}

	public:
		operator uint64_t() const { return m_UUID; }

	private:
		u64 m_UUID;
	};
}

namespace std 
{
	template <typename T> struct hash;

	template<>
	struct hash<tur::UUID>
	{
		size_t operator()(const tur::UUID& uuid) const
		{
			return static_cast<uint64_t>(uuid);
		}
	};
}