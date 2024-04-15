#include "control/keyboard_config.hpp"

KeyboardConfig::KeyboardConfig():
	m_keymap()
{
	m_keymap[SDLK_LEFT] = {0, Control::LEFT};
	m_keymap[SDLK_RIGHT] = {0, Control::RIGHT};
	m_keymap[SDLK_UP] = {0, Control::UP};
	m_keymap[SDLK_DOWN] = {0, Control::DOWN};

	m_keymap[SDLK_SPACE] = {0, Control::SPACE};
	m_keymap[SDLK_LCTRL] = {0, Control::ACTION};
	m_keymap[SDLK_ESCAPE] = {0, Control::ESCAPE};
	m_keymap[SDLK_KP_ENTER] = {0, Control::MENU_SELECT};
	m_keymap[SDLK_RETURN] = {0, Control::MENU_SELECT};


	m_keymap[SDLK_a] = {0, Control::LEFT};
	m_keymap[SDLK_d] = {0, Control::RIGHT};
	m_keymap[SDLK_w] = {0, Control::UP};
	m_keymap[SDLK_s] = {0, Control::DOWN};
}

SDL_Keycode KeyboardConfig::reversemap_key(int player, Control c) const {
	for (const auto& key : m_keymap) {
		if (key.second == PlayerControl{player, c}) {
			return key.first;
		}
	}
	return SDLK_UNKNOWN;
}