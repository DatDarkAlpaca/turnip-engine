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
			: m_UUID(generate_random<u64>(std::numeric_limits<u64>::min(), std::numeric_limits<u64>::max() - 1))
		{

		}

		UUID(u64 uuid)
			: m_UUID(uuid)
		{
		}

	public:
		operator u64() const { return m_UUID; }

	public:
		bool is_valid() const { return m_UUID != std::numeric_limits<u64>::max(); }

	public:
		u64& data() { return m_UUID; }
		u64 data() const { return m_UUID; }

	private:
		u64 m_UUID = std::numeric_limits<u64>::max();
	};

	static inline UUID invalid_uuid = UUID(std::numeric_limits<u64>::max());
}

namespace std 
{
	template <typename T> struct hash;

	template<>
	struct hash<tur::UUID>
	{
		size_t operator()(const tur::UUID& uuid) const
		{
			return uuid.data();
		}
	};
}