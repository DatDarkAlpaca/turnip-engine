#pragma once
#include "Common.h"
#include "Events_GLFW.h"

#ifdef TUR_DEBUG
	#define __TRANSLATE_MOUSE(buttonMacro) case buttonMacro: return #buttonMacro;
	#define __TRANSLATE_KEY(keyMacro) if (key == keyMacro) return #keyMacro;
#endif

namespace tur
{
#ifdef TUR_DEBUG
	inline constexpr const char* DebugMouseButtonName(MouseButton button)
	{
		switch (button)
		{
			__TRANSLATE_MOUSE(MouseButton::MOUSE_BUTTON_UNKNOWN);
			__TRANSLATE_MOUSE(MouseButton::MOUSE_BUTTON_LEFT);
			__TRANSLATE_MOUSE(MouseButton::MOUSE_BUTTON_RIGHT);
			__TRANSLATE_MOUSE(MouseButton::MOUSE_BUTTON_MIDDLE);
			__TRANSLATE_MOUSE(MouseButton::MOUSE_BUTTON_4);
			__TRANSLATE_MOUSE(MouseButton::MOUSE_BUTTON_5);
			__TRANSLATE_MOUSE(MouseButton::MOUSE_BUTTON_6);
			__TRANSLATE_MOUSE(MouseButton::MOUSE_BUTTON_LAST);
		}
	}

	inline std::string DebugMods(KeyboardMods mods)
	{
		std::vector<std::string> modResults;

		if ((int)mods & GLFW_MOD_SHIFT)
			modResults.push_back("SHIFT");

		if ((int)mods & GLFW_MOD_CONTROL)
			modResults.push_back("CTRL");

		if ((int)mods & GLFW_MOD_ALT)
			modResults.push_back("ALT");

		if ((int)mods & GLFW_MOD_SUPER)
			modResults.push_back("SUPER");

		if ((int)mods & GLFW_MOD_CAPS_LOCK)
			modResults.push_back("CAPS");

		if ((int)mods & GLFW_MOD_NUM_LOCK)
			modResults.push_back("NUM_LOCK");

		if (modResults.size() == 0)
			return "NONE";

		if (modResults.size() == 1)
			return modResults.back().c_str();

		std::string str = "";
		for (size_t i = 0; i != modResults.size(); ++i)
		{
			if (i == modResults.size() - 1)
				str += modResults[i];
			else
				str += modResults[i] + " | ";
		}

		return str.c_str();
	}

	inline constexpr const char* DebugKeyName(Key key)
	{
		__TRANSLATE_KEY(Key::KEY_UNKNOWN);
		__TRANSLATE_KEY(Key::KEY_SPACE);
		__TRANSLATE_KEY(Key::KEY_APOSTROPHE);
		__TRANSLATE_KEY(Key::KEY_COMMA);
		__TRANSLATE_KEY(Key::KEY_MINUS);
		__TRANSLATE_KEY(Key::KEY_PERIOD);
		__TRANSLATE_KEY(Key::KEY_SLASH);
		__TRANSLATE_KEY(Key::KEY_0);
		__TRANSLATE_KEY(Key::KEY_1);
		__TRANSLATE_KEY(Key::KEY_2);
		__TRANSLATE_KEY(Key::KEY_3);
		__TRANSLATE_KEY(Key::KEY_4);
		__TRANSLATE_KEY(Key::KEY_5);
		__TRANSLATE_KEY(Key::KEY_6);
		__TRANSLATE_KEY(Key::KEY_7);
		__TRANSLATE_KEY(Key::KEY_8);
		__TRANSLATE_KEY(Key::KEY_9);
		__TRANSLATE_KEY(Key::KEY_SEMICOLON);
		__TRANSLATE_KEY(Key::KEY_EQUAL);
		__TRANSLATE_KEY(Key::KEY_A);
		__TRANSLATE_KEY(Key::KEY_B);
		__TRANSLATE_KEY(Key::KEY_C);
		__TRANSLATE_KEY(Key::KEY_D);
		__TRANSLATE_KEY(Key::KEY_E);
		__TRANSLATE_KEY(Key::KEY_F);
		__TRANSLATE_KEY(Key::KEY_G);
		__TRANSLATE_KEY(Key::KEY_H);
		__TRANSLATE_KEY(Key::KEY_I);
		__TRANSLATE_KEY(Key::KEY_J);
		__TRANSLATE_KEY(Key::KEY_K);
		__TRANSLATE_KEY(Key::KEY_L);
		__TRANSLATE_KEY(Key::KEY_M);
		__TRANSLATE_KEY(Key::KEY_N);
		__TRANSLATE_KEY(Key::KEY_O);
		__TRANSLATE_KEY(Key::KEY_P);
		__TRANSLATE_KEY(Key::KEY_Q);
		__TRANSLATE_KEY(Key::KEY_R);
		__TRANSLATE_KEY(Key::KEY_S);
		__TRANSLATE_KEY(Key::KEY_T);
		__TRANSLATE_KEY(Key::KEY_U);
		__TRANSLATE_KEY(Key::KEY_V);
		__TRANSLATE_KEY(Key::KEY_W);
		__TRANSLATE_KEY(Key::KEY_X);
		__TRANSLATE_KEY(Key::KEY_Y);
		__TRANSLATE_KEY(Key::KEY_Z);
		__TRANSLATE_KEY(Key::KEY_LEFT_BRACKET);
		__TRANSLATE_KEY(Key::KEY_BACKSLASH);
		__TRANSLATE_KEY(Key::KEY_RIGHT_BRACKET);
		__TRANSLATE_KEY(Key::KEY_GRAVE_ACCENT);
		__TRANSLATE_KEY(Key::KEY_WORLD_1);
		__TRANSLATE_KEY(Key::KEY_WORLD_2);
		__TRANSLATE_KEY(Key::KEY_ESCAPE);
		__TRANSLATE_KEY(Key::KEY_ENTER);
		__TRANSLATE_KEY(Key::KEY_TAB);
		__TRANSLATE_KEY(Key::KEY_BACKSPACE);
		__TRANSLATE_KEY(Key::KEY_INSERT);
		__TRANSLATE_KEY(Key::KEY_DELETE);
		__TRANSLATE_KEY(Key::KEY_RIGHT);
		__TRANSLATE_KEY(Key::KEY_LEFT);
		__TRANSLATE_KEY(Key::KEY_DOWN);
		__TRANSLATE_KEY(Key::KEY_UP);
		__TRANSLATE_KEY(Key::KEY_PAGE_UP);
		__TRANSLATE_KEY(Key::KEY_PAGE_DOWN);
		__TRANSLATE_KEY(Key::KEY_HOME);
		__TRANSLATE_KEY(Key::KEY_END);
		__TRANSLATE_KEY(Key::KEY_CAPS_LOCK);
		__TRANSLATE_KEY(Key::KEY_SCROLL_LOCK);
		__TRANSLATE_KEY(Key::KEY_NUM_LOCK);
		__TRANSLATE_KEY(Key::KEY_PRINT_SCREEN);
		__TRANSLATE_KEY(Key::KEY_PAUSE);
		__TRANSLATE_KEY(Key::KEY_F1);
		__TRANSLATE_KEY(Key::KEY_F2);
		__TRANSLATE_KEY(Key::KEY_F3);
		__TRANSLATE_KEY(Key::KEY_F4);
		__TRANSLATE_KEY(Key::KEY_F5);
		__TRANSLATE_KEY(Key::KEY_F6);
		__TRANSLATE_KEY(Key::KEY_F7);
		__TRANSLATE_KEY(Key::KEY_F8);
		__TRANSLATE_KEY(Key::KEY_F9);
		__TRANSLATE_KEY(Key::KEY_F10);
		__TRANSLATE_KEY(Key::KEY_F11);
		__TRANSLATE_KEY(Key::KEY_F12);
		__TRANSLATE_KEY(Key::KEY_F13);
		__TRANSLATE_KEY(Key::KEY_F14);
		__TRANSLATE_KEY(Key::KEY_F15);
		__TRANSLATE_KEY(Key::KEY_F16);
		__TRANSLATE_KEY(Key::KEY_F17);
		__TRANSLATE_KEY(Key::KEY_F18);
		__TRANSLATE_KEY(Key::KEY_F19);
		__TRANSLATE_KEY(Key::KEY_F20);
		__TRANSLATE_KEY(Key::KEY_F21);
		__TRANSLATE_KEY(Key::KEY_F22);
		__TRANSLATE_KEY(Key::KEY_F23);
		__TRANSLATE_KEY(Key::KEY_F24);
		__TRANSLATE_KEY(Key::KEY_F25);
		__TRANSLATE_KEY(Key::KEY_KP_0);
		__TRANSLATE_KEY(Key::KEY_KP_1);
		__TRANSLATE_KEY(Key::KEY_KP_2);
		__TRANSLATE_KEY(Key::KEY_KP_3);
		__TRANSLATE_KEY(Key::KEY_KP_4);
		__TRANSLATE_KEY(Key::KEY_KP_5);
		__TRANSLATE_KEY(Key::KEY_KP_6);
		__TRANSLATE_KEY(Key::KEY_KP_7);
		__TRANSLATE_KEY(Key::KEY_KP_8);
		__TRANSLATE_KEY(Key::KEY_KP_9);
		__TRANSLATE_KEY(Key::KEY_KP_DECIMAL);
		__TRANSLATE_KEY(Key::KEY_KP_DIVIDE);
		__TRANSLATE_KEY(Key::KEY_KP_MULTIPLY);
		__TRANSLATE_KEY(Key::KEY_KP_SUBTRACT);
		__TRANSLATE_KEY(Key::KEY_KP_ADD);
		__TRANSLATE_KEY(Key::KEY_KP_ENTER);
		__TRANSLATE_KEY(Key::KEY_KP_EQUAL);
		__TRANSLATE_KEY(Key::KEY_LEFT_SHIFT);
		__TRANSLATE_KEY(Key::KEY_LEFT_CONTROL);
		__TRANSLATE_KEY(Key::KEY_LEFT_ALT);
		__TRANSLATE_KEY(Key::KEY_LEFT_SUPER);
		__TRANSLATE_KEY(Key::KEY_RIGHT_SHIFT);
		__TRANSLATE_KEY(Key::KEY_RIGHT_CONTROL);
		__TRANSLATE_KEY(Key::KEY_RIGHT_ALT);
		__TRANSLATE_KEY(Key::KEY_RIGHT_SUPER);
		__TRANSLATE_KEY(Key::KEY_MENU);
		__TRANSLATE_KEY(Key::KEY_LAST);
}

#else
	inline constexpr const char* DebugMouseButtonName(MouseButton button) { }

	inline std::string DebugMods(KeyboardMods mods) { }

	inline constexpr const char* DebugKeyName(Key key) { }

#endif	
}

