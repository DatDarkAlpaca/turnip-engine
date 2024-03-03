#pragma once
#include <bitset>
#include <numeric>
#include <type_traits>

template<typename T>
struct StateTraits;

#define DECLARE_STATE(StateName)																\
template<>																						\
struct StateTraits<StateName>																	\
{																								\
	static const StateName max = StateName::MAX_ITEMS;											\
};																								\
inline StateName operator| (StateName rhs, StateName lhs)										\
{																								\
	return static_cast<StateName>(static_cast<uint64_t>(rhs) | static_cast<uint64_t>(lhs));		\
}																								\
inline StateName operator& (StateName rhs, StateName lhs)										\
{																								\
	return static_cast<StateName>(static_cast<uint64_t>(rhs) & static_cast<uint64_t>(lhs));		\
}

template<typename T>
class State
{
	using StateType = typename std::underlying_type<T>::type;

public:
	State() = default;

	State(T state)
	{
		Set(state);
	}

	State& operator= (T state)
	{
		Set(state);
		return *this;
	}

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
	
	T TypeAt(uint64_t position) const
	{
		auto value = m_Bitset[position];
		if (value == 0)
			return T{ 0 };

		return static_cast<T>(1 << value * position);
	}

	uint64_t Count() const 
	{
		return static_cast<uint64_t>(StateTraits<T>::max);
	}

public:
	auto operator[] (T position)
	{
		return m_Bitset.test(Bit(position));
	}

	const auto operator[] (T position) const
	{
		return m_Bitset.test(Bit(position));
	}

	auto operator[] (uint64_t position)
	{
		return m_Bitset[position];
	}
	
	const auto operator[] (uint64_t position) const
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
		return static_cast<uint64_t>(value);
	}

private:
	std::bitset<static_cast<StateType>(StateTraits<T>::max)> m_Bitset;
};