#include "control/input_manager.hpp"

#include "control/keyboard_manager.hpp"
#include "control/mouse_button_manager.hpp"
#include "control/mouse_motion_manager.hpp"

InputManager::InputManager(KeyboardConfig& keyboard_config, MouseConfig& mouse_config) :
	m_controllers(),
	keyboard_manager(std::make_unique<KeyboardManager>(this, keyboard_config)),
	mouse_button_manager(std::make_unique<MouseButtonManager>(this, mouse_config)),
	mouse_motion_manager(std::make_unique<MouseMotionManager>(this))
{
	m_controllers.push_back(std::make_unique<Controller>());
}

InputManager::~InputManager()
{}


void InputManager::update() {
	for (auto& controller : m_controllers) {
		controller->update();
	}
}

void InputManager::reset() {
	for (auto& controller : m_controllers) {
		controller->reset();
	}
}

void InputManager::process_event(const SDL_Event& event) {
	switch (event.type) {
		case SDL_KEYUP:
		case SDL_KEYDOWN:
			keyboard_manager->process_key_event(event.key);
			break;

		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEBUTTONDOWN:
			mouse_button_manager->process_mouse_button_event(event.button);
			break;

		case SDL_MOUSEMOTION:
			mouse_motion_manager->process_mouse_motion_event(event.motion);
			
		default:
			break;
	}
}

Controller& InputManager::get_controller(int player_id) {
	return *m_controllers[player_id];
}

int InputManager::get_num_users() const { 
	return static_cast<int>(m_controllers.size());
}