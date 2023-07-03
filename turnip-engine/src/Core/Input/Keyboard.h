#pragma once
#include "pch.h"
#include "Key.h"
#include "State.h"
#include "Mods.h"
#include "Logger/Logger.h"

namespace tur
{
	class Keyboard
	{
	public:
		static void SetKeyState(int key, int scancode, int action, int mods)
		{
			m_Keys[key] = State(action);
			m_Mods = mods;
		}

	public:
		static State Key(Key key) { return m_Keys[(int)key]; }

		static int Mods() { return m_Mods; }

	private:
		static inline std::array<State, (int)Key::AMOUNT> m_Keys;
		static inline int m_Mods;
	};
}