#include "control/mouse_config.hpp"

MouseConfig::MouseConfig():
	m_keymap()
{
	m_keymap[SDL_BUTTON_LEFT] = {0, Control::ATTACK};
}