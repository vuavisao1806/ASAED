#include "control/mouse_button_manager.hpp"

#include "control/input_manager.hpp"

MouseButtonManager::MouseButtonManager(InputManager* parent, MouseConfig& mouse_config) :
	m_parent(parent),
	m_mouse_config(mouse_config)
{}

void MouseButtonManager::process_mouse_button_event(const SDL_MouseButtonEvent& event) {
	auto key_mappping = m_mouse_config.m_keymap.find(event.button);

	if (key_mappping == m_mouse_config.m_keymap.end()) {
		return;
	}
	auto control = key_mappping->second;
	bool value = (event.type == SDL_MOUSEBUTTONDOWN);

	if (control.player >= m_parent->get_num_users()) return;
	m_parent->get_controller(control.player).set_control(control.control, value);
}