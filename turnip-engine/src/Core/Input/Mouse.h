#pragma once
#include "pch.h"
#include "State.h"
#include "MouseButton.h"

namespace tur
{
	class Mouse
	{
	public:
		static void SetPosition(const glm::vec2& newPosition)
		{
			positionLast = position;
			position = newPosition;
		}

		static void SetButtonState(int button, int action, int mods)
		{
			m_Buttons[button] = State(action);
		}

		static void SetScrollOffset(double x, double y)
		{
			scrollOffset = { x, y };
		}

		static void Clear()
		{
			scrollOffset = { 0, 0 };
		}

	public:
		static State Button(MouseButton button) { return m_Buttons[(int)button]; }

	public:
		static inline constexpr int AmountButtons = 8;

	private:
		static inline std::array<State, (int)MouseButton::AMOUNT> m_Buttons;

	public:
		static inline glm::vec2 position = glm::vec2(0), positionLast = glm::vec2(0);
		static inline glm::vec2 scrollOffset = glm::vec2(0);
	};
}