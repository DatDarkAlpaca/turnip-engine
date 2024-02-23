#pragma once
#include <bitset>
#include <numeric>
#include <type_traits>

#define DECLARE_STATE(StateName)						\
template<>												\
struct StateTraits<StateName>							\
{														\
	static const StateName max = StateName::MAX_ITEMS;  \
};

template<typename T>
struct StateTraits;

template<typename T>
class State
{
	using StateType = typename std::underlying_type<T>::type;

public:
	explicit State() = default;

	State(const std::bitset<static_cast<StateType>(StateTraits<T>::max)>& bitset)
		: m_Bitset(bitset)
	{

	}

public:
	State& Set(T position, bool value = true)
	{
		m_Bitset.set(Bit(position), value);
		return *this;
	}

public:
	auto operator[] (T position)
	{
		return m_Bitset[Bit(position)];
	}

	auto operator[] (uint64_t position)
	{
		return m_Bitset[position];
	}

	State operator| (const State& lhs)
	{
		return Bitset() | lhs.Bitset();
	}

	State operator& (const State& lhs)
	{
		return Bitset() & lhs.Bitset();
	}

public:
	auto Bitset() const { return m_Bitset; }

private:
	inline StateType Value(T value) const
	{
		return static_cast<StateType>(value);
	}

	inline uint64_t Bit(T value) const
	{
		return static_cast<uint64_t>(value) >> 1;
	}

private:
	std::bitset<static_cast<StateType>(StateTraits<T>::max)> m_Bitset;
};