#pragma once
#include <numeric>
#include "Util/Random/Random.hpp"

namespace tur
{
	class UUID
	{
	public:
		UUID()
			: m_UUID(GenerateRandom<std::uint64_t>())
		{

		}

	public:
		operator uint64_t() const { return m_UUID; }

	private:
		std::uint64_t m_UUID;
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
			return (uint64_t)uuid;
		}
	};
}