#include "control/keyboard_manager.hpp"

#include "control/input_manager.hpp"

KeyboardManager::KeyboardManager(InputManager* parent, KeyboardConfig& keyboard_config) :
	m_parent(parent),
	m_keyboard_config(keyboard_config)
{}

void KeyboardManager::process_key_event(const SDL_KeyboardEvent& event) {
	auto key_mappping = m_keyboard_config.m_keymap.find(event.keysym.sym);

	if (key_mappping == m_keyboard_config.m_keymap.end()) {
		return;
	}
	auto control = key_mappping->second;
	bool value = (event.type == SDL_KEYDOWN);

	if (control.player >= m_parent->get_num_users()) return;
	m_parent->get_controller(control.player).set_control(control.control, value);
}