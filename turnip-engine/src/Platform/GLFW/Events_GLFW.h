#pragma once
#include "Core/Defines.h"
#include "Core/Event/KeyboardEvents/KeyboardEvent.h"
#include "Core/Event/MouseEvents/MouseEvent.h"

#define GLFW_INPUT

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace tur
{
	inline constexpr Key TranslateKey(GLFW_INPUT uint32_t key)
	{
		switch (key)
		{
			case GLFW_KEY_SPACE:
				return Key::KEY_SPACE;
			case GLFW_KEY_APOSTROPHE:
				return Key::KEY_APOSTROPHE;
			case GLFW_KEY_COMMA:
				return Key::KEY_COMMA;
			case GLFW_KEY_MINUS:
				return Key::KEY_MINUS;
			case GLFW_KEY_PERIOD:
				return Key::KEY_PERIOD;
			case GLFW_KEY_SLASH:
				return Key::KEY_SLASH;
			case GLFW_KEY_0:
				return Key::KEY_0;
			case GLFW_KEY_1:
				return Key::KEY_1;
			case GLFW_KEY_2:
				return Key::KEY_2;
			case GLFW_KEY_3:
				return Key::KEY_3;
			case GLFW_KEY_4:
				return Key::KEY_4;
			case GLFW_KEY_5:
				return Key::KEY_5;
			case GLFW_KEY_6:
				return Key::KEY_6;
			case GLFW_KEY_7:
				return Key::KEY_7;
			case GLFW_KEY_8:
				return Key::KEY_8;
			case GLFW_KEY_9:
				return Key::KEY_9;
			case GLFW_KEY_SEMICOLON:
				return Key::KEY_SEMICOLON;
			case GLFW_KEY_EQUAL:
				return Key::KEY_EQUAL;
			case GLFW_KEY_A:
				return Key::KEY_A;
			case GLFW_KEY_B:
				return Key::KEY_B;
			case GLFW_KEY_C:
				return Key::KEY_C;
			case GLFW_KEY_D:
				return Key::KEY_D;
			case GLFW_KEY_E:
				return Key::KEY_E;
			case GLFW_KEY_F:
				return Key::KEY_F;
			case GLFW_KEY_G:
				return Key::KEY_G;
			case GLFW_KEY_H:
				return Key::KEY_H;
			case GLFW_KEY_I:
				return Key::KEY_I;
			case GLFW_KEY_J:
				return Key::KEY_J;
			case GLFW_KEY_K:
				return Key::KEY_K;
			case GLFW_KEY_L:
				return Key::KEY_L;
			case GLFW_KEY_M:
				return Key::KEY_M;
			case GLFW_KEY_N:
				return Key::KEY_N;
			case GLFW_KEY_O:
				return Key::KEY_O;
			case GLFW_KEY_P:
				return Key::KEY_P;
			case GLFW_KEY_Q:
				return Key::KEY_Q;
			case GLFW_KEY_R:
				return Key::KEY_R;
			case GLFW_KEY_S:
				return Key::KEY_S;
			case GLFW_KEY_T:
				return Key::KEY_T;
			case GLFW_KEY_U:
				return Key::KEY_U;
			case GLFW_KEY_V:
				return Key::KEY_V;
			case GLFW_KEY_W:
				return Key::KEY_W;
			case GLFW_KEY_X:
				return Key::KEY_X;
			case GLFW_KEY_Y:
				return Key::KEY_Y;
			case GLFW_KEY_Z:
				return Key::KEY_Z;
			case GLFW_KEY_LEFT_BRACKET:
				return Key::KEY_LEFT_BRACKET;
			case GLFW_KEY_BACKSLASH:
				return Key::KEY_BACKSLASH;
			case GLFW_KEY_RIGHT_BRACKET:
				return Key::KEY_RIGHT_BRACKET;
			case GLFW_KEY_GRAVE_ACCENT:
				return Key::KEY_GRAVE_ACCENT;
			case GLFW_KEY_ESCAPE:
				return Key::KEY_ESCAPE;
			case GLFW_KEY_ENTER:
				return Key::KEY_ENTER;
			case GLFW_KEY_TAB:
				return Key::KEY_TAB;
			case GLFW_KEY_BACKSPACE:
				return Key::KEY_BACKSPACE;
			case GLFW_KEY_INSERT:
				return Key::KEY_INSERT;
			case GLFW_KEY_DELETE:
				return Key::KEY_DELETE;
			case GLFW_KEY_RIGHT:
				return Key::KEY_RIGHT;
			case GLFW_KEY_LEFT:
				return Key::KEY_LEFT;
			case GLFW_KEY_DOWN:
				return Key::KEY_DOWN;
			case GLFW_KEY_UP:
				return Key::KEY_UP;
			case GLFW_KEY_PAGE_UP:
				return Key::KEY_PAGE_UP;
			case GLFW_KEY_PAGE_DOWN:
				return Key::KEY_PAGE_DOWN;
			case GLFW_KEY_HOME:
				return Key::KEY_HOME;
			case GLFW_KEY_END:
				return Key::KEY_END;
			case GLFW_KEY_CAPS_LOCK:
				return Key::KEY_CAPS_LOCK;
			case GLFW_KEY_SCROLL_LOCK:
				return Key::KEY_SCROLL_LOCK;
			case GLFW_KEY_NUM_LOCK:
				return Key::KEY_NUM_LOCK;
			case GLFW_KEY_PRINT_SCREEN:
				return Key::KEY_PRINT_SCREEN;
			case GLFW_KEY_PAUSE:
				return Key::KEY_PAUSE;
			case GLFW_KEY_F1:
				return Key::KEY_F1;
			case GLFW_KEY_F2:
				return Key::KEY_F2;
			case GLFW_KEY_F3:
				return Key::KEY_F3;
			case GLFW_KEY_F4:
				return Key::KEY_F4;
			case GLFW_KEY_F5:
				return Key::KEY_F5;
			case GLFW_KEY_F6:
				return Key::KEY_F6;
			case GLFW_KEY_F7:
				return Key::KEY_F7;
			case GLFW_KEY_F8:
				return Key::KEY_F8;
			case GLFW_KEY_F9:
				return Key::KEY_F9;
			case GLFW_KEY_F10:
				return Key::KEY_F10;
			case GLFW_KEY_F11:
				return Key::KEY_F11;
			case GLFW_KEY_F12:
				return Key::KEY_F12;
			case GLFW_KEY_F13:
				return Key::KEY_F13;
			case GLFW_KEY_F14:
				return Key::KEY_F14;
			case GLFW_KEY_F15:
				return Key::KEY_F15;
			case GLFW_KEY_F16:
				return Key::KEY_F16;
			case GLFW_KEY_F17:
				return Key::KEY_F17;
			case GLFW_KEY_F18:
				return Key::KEY_F18;
			case GLFW_KEY_F19:
				return Key::KEY_F19;
			case GLFW_KEY_F20:
				return Key::KEY_F20;
			case GLFW_KEY_F21:
				return Key::KEY_F21;
			case GLFW_KEY_F22:
				return Key::KEY_F22;
			case GLFW_KEY_F23:
				return Key::KEY_F23;
			case GLFW_KEY_F24:
				return Key::KEY_F24;
			case GLFW_KEY_KP_0:
				return Key::KEY_KP_0;
			case GLFW_KEY_KP_1:
				return Key::KEY_KP_1;
			case GLFW_KEY_KP_2:
				return Key::KEY_KP_2;
			case GLFW_KEY_KP_3:
				return Key::KEY_KP_3;
			case GLFW_KEY_KP_4:
				return Key::KEY_KP_4;
			case GLFW_KEY_KP_5:
				return Key::KEY_KP_5;
			case GLFW_KEY_KP_6:
				return Key::KEY_KP_6;
			case GLFW_KEY_KP_7:
				return Key::KEY_KP_7;
			case GLFW_KEY_KP_8:
				return Key::KEY_KP_8;
			case GLFW_KEY_KP_9:
				return Key::KEY_KP_9;
			case GLFW_KEY_KP_DECIMAL:
				return Key::KEY_KP_DECIMAL;
			case GLFW_KEY_KP_DIVIDE:
				return Key::KEY_KP_DIVIDE;
			case GLFW_KEY_KP_MULTIPLY:
				return Key::KEY_KP_MULTIPLY;
			case GLFW_KEY_KP_SUBTRACT:
				return Key::KEY_KP_SUBTRACT;
			case GLFW_KEY_KP_ADD:
				return Key::KEY_KP_ADD;
			case GLFW_KEY_KP_ENTER:
				return Key::KEY_KP_ENTER;
			case GLFW_KEY_KP_EQUAL:
				return Key::KEY_KP_EQUAL;
			case GLFW_KEY_LEFT_SHIFT:
				return Key::KEY_LEFT_SHIFT;
			case GLFW_KEY_LEFT_CONTROL:
				return Key::KEY_LEFT_CONTROL;
			case GLFW_KEY_LEFT_ALT:
				return Key::KEY_LEFT_ALT;
			case GLFW_KEY_LEFT_SUPER:
				return Key::KEY_LEFT_SUPER;
			case GLFW_KEY_RIGHT_SHIFT:
				return Key::KEY_RIGHT_SHIFT;
			case GLFW_KEY_RIGHT_CONTROL:
				return Key::KEY_RIGHT_CONTROL;
			case GLFW_KEY_RIGHT_ALT:
				return Key::KEY_RIGHT_ALT;
			case GLFW_KEY_RIGHT_SUPER:
				return Key::KEY_RIGHT_SUPER;
			case GLFW_KEY_MENU:
				return Key::KEY_MENU;
			default:
				return Key::KEY_UNKNOWN;
		}
	}

	inline constexpr KeyboardMods TranslateKeyMods(GLFW_INPUT uint32_t mods)
	{
		uint32_t result = 0;

		if (mods & GLFW_MOD_SHIFT)
			result |= (uint32_t)KeyboardMods::KEY_MOD_SHIFT;

		if (mods & GLFW_MOD_CONTROL)
			result |= (uint32_t)KeyboardMods::KEY_MOD_CTRL;

		if (mods & GLFW_MOD_ALT)
			result |= (uint32_t)KeyboardMods::KEY_MOD_ALT;

		if (mods & GLFW_MOD_SUPER)
			result |= (uint32_t)KeyboardMods::KEY_MOD_SUPER;

		if (mods & GLFW_MOD_CAPS_LOCK)
			result |= (uint32_t)KeyboardMods::KEY_MOD_CAPS_LOCK;

		if (mods & GLFW_MOD_NUM_LOCK)
			result |= (uint32_t)KeyboardMods::KEY_MOD_NUM_LOCK;

		return (KeyboardMods)result;
	}

	inline constexpr MouseButton TranslateMouseButton(uint32_t mouseButton)
	{
		switch (mouseButton)
		{
			case GLFW_MOUSE_BUTTON_1:
				return MouseButton::MOUSE_BUTTON_1;

			case GLFW_MOUSE_BUTTON_2:
				return MouseButton::MOUSE_BUTTON_2;

			case GLFW_MOUSE_BUTTON_3:
				return MouseButton::MOUSE_BUTTON_3;

			case GLFW_MOUSE_BUTTON_4:
				return MouseButton::MOUSE_BUTTON_4;

			case GLFW_MOUSE_BUTTON_5:
				return MouseButton::MOUSE_BUTTON_5;

			case GLFW_MOUSE_BUTTON_6:
				return MouseButton::MOUSE_BUTTON_6;

			case GLFW_MOUSE_BUTTON_7:
				return MouseButton::MOUSE_BUTTON_7;

			case GLFW_MOUSE_BUTTON_8:
				return MouseButton::MOUSE_BUTTON_8;

			default:
				return MouseButton::MOUSE_BUTTON_UNKNOWN;
		}
	}
}