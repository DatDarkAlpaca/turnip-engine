#pragma once
#include <bitset>
#include <type_traits>

namespace tur
{
	template<typename T>
	struct StateTraits;

	template<typename T>
	class State
	{
		using StateType = typename std::underlying_type<T>::type;

	public:
		bool Test(T pos) const
		{
			return m_Bitset.test(Value(pos));
		}

		State& Reset(T pos)
		{
			m_Bitset.reset(Value(pos));
			return *this;
		}

		State& Flip(T pos)
		{
			m_Bitset.flip(Value(pos));
			return *this;
		}

	public:
		State operator| (State lhs)
		{
			State state;
			state.m_Bitset = m_Bitset | lhs.m_Bitset;

			return state;
		}

		State operator& (State lhs)
		{
			State state;
			state.m_Bitset = m_Bitset & lhs.m_Bitset;

			return state;
		}

	private:
		StateType Value(T v) const
		{
			return static_cast<StateType>(v);
		}

	public:
		std::bitset<static_cast<StateType>(StateTraits<T>::max)> m_Bitset;
	};
}